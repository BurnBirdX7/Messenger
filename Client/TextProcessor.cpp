#include "TextProcessor.hpp"

#include <utility>

TextProcessor::TextProcessor(Client& client)
    : mTasker(client)
    , mContainer()
    , mCurrentCommandSet(nullptr)
    , mStopped(false)
{
    if (client.isConnected())
        mCurrentCommandSet = &mConnected_CommandSet;
    else
        mCurrentCommandSet = &mDisconnected_CommandSet;

    _initDisconnected();
    _initConnected();
    _initAuthorized();
}

void TextProcessor::run()
{
    { // write
        std::scoped_lock lock(mIOMutex);
        std::cout << "Text Processor" << std::endl;
    } // ! read

    while (!mStopped) {
        std::string line;
        { // read
            std::scoped_lock lock(mIOMutex);
            std::cout << " > ";
            std::getline(std::cin, line, '\n');
        } // ! read
        mCurrentCommandSet->execute(line);
    }
}

void TextProcessor::runChat(const Chat& chat)
{

}

void TextProcessor::clientNotificationHandler(uint8_t header, ConstBuffer buffer)
{
    if (header == Purpose::NTF_CHAT_MSG) {
        // Unpack
        ChatMessagePack pack;
        pack.fillFromBuffer(buffer);
        mContainer.addMessagePack(pack);
    }
    else if (header == Purpose::NTF_JOIN_CHAT) {
        ChatInfo info;
        info.fillFromBuffer(buffer);
        mContainer.addChat(Chat(info));
    }
    else if (header == Purpose::NTF_LEAVE_CHAT) {
        ChatInfo info;
        info.fillFromBuffer(buffer);
        mContainer.removeChat(info.getId());
    }
}

void TextProcessor::clientStateHandler(Client::State state)
{
    switch (state) {
        case Client::State::DISCONNECTED:
            mCurrentCommandSet = &mDisconnected_CommandSet;
            break;
        case Client::State::ABLE_TO_CONNECT:
            mTasker.getClient().connect();
            break;
        case Client::State::CONNECTED:
            mCurrentCommandSet = &mConnected_CommandSet;
            break;
        case Client::State::AUTHORIZED:
            mCurrentCommandSet = &mAuthorized_CommandSet;
            break;
    }
}

/* GENERAL ACTIONS: */

void TextProcessor::reportSuccess(const std::string& msg)
{
    std::scoped_lock lock(mIOMutex);
    std::cout << "Success: " << msg << std::endl;
}

void TextProcessor::reportProblem(const std::string& str)
{
    std::scoped_lock lock(mIOMutex);
    std::cout << "Failure: " << str << std::endl;
}

void TextProcessor::reportProblem(const std::string& msg, CompletionError ec, ConstBuffer reason)
{
    BufferDecomposer decomposer(reason);
    std::scoped_lock lock(mIOMutex);
    std::cout << msg << ": [Code: " << ec << "] ";
    std::cout << decomposer.get<std::string>() << std::endl;
}

/* ADDITIONAL */

void TextProcessor::reportProblem(int errorCode)
{
    std::string error_str;

    switch (errorCode) {
        case NO_SUCH_COMMAND:
            error_str = "No such command";
            break;
        case TOO_FEW_PARAMS:
            error_str = "Too few parameters";
            break;
        case TOO_MANY_PARAMS:
            error_str = "Too many parameters";
            break;
        case WRONG_PARAMS:
            error_str = "Incorrect parameters";
            break;
        case CANT_GET_THE_CHAT:
            error_str = "Couldn't find that chat";
            break;
        default:
            error_str = "Unknown error";
    }

    std::scoped_lock lock(mIOMutex);
    std::cout << "Failure: " << error_str << std::endl;

}

void TextProcessor::authorize_from_login(ConstBuffer buffer)
{
    BufferDecomposer decomposer(buffer);
    decomposer
        .extract(mSessionId)
        .extract(mSessionHash)
        .extract(mUserId);
}

void TextProcessor::authorize_from_session(ConstBuffer buffer)
{
    // TODO: ^
}

void TextProcessor::_join_chat(const std::string& name, const std::string& password)
{
    auto shInfo = std::make_shared<Commons::Data::ChatInfo>();

    auto join_handler = [this, shInfo] (CompletionError ec, ConstBuffer buffer) {
        if (ec == CompletionError::OK) {
            mContainer.addChat(Chat(*shInfo));
            reportSuccess("Joined");
        }
        else {
            reportProblem("Couldn't join chat", ec, buffer);
        }

    };

    auto get_chat_handler = [this, join_handler, password, shInfo] (CompletionError ec, ConstBuffer buffer) {
        if (ec == CompletionError::OK) {
            shInfo->fillFromBuffer(buffer);
            if (shInfo->hasPassword() && password.empty())
                reportProblem("The chat requires password");
            else if (shInfo->hasPassword())
                mTasker.joinChat(shInfo->getId(), password, join_handler);
            else
                mTasker.joinChat(shInfo->getId(), join_handler);
        }
        else {
            reportProblem("Couldn't load chat data", ec, buffer);
        }
    };

    mTasker.getChatByName(name, get_chat_handler);
}

/* COMMAND SETS: */

void TextProcessor::command_login(const std::string& command)
{
    std::string login, password;

    std::stringstream ss(command);
    ss >> login >> password;

    if (login.empty() || password.empty()) {
        reportProblem(Error::TOO_FEW_PARAMS);
    }

    mTasker.login(login, password, [this] (CompletionError ec, ConstBuffer buffer) {
        if (ec == CompletionError::OK) {
            reportSuccess("Successful login!");
            authorize_from_login(buffer);
        }
        else {
            reportProblem("Login failed", ec, buffer);
        }
    });

}

void TextProcessor::command_join(const std::string& command)
{
    std::string chatname, password;
    std::stringstream ss(command);
    ss >> chatname >> password;

    if (chatname.empty()) {
        reportProblem(TOO_FEW_PARAMS);
        return;
    }

    _join_chat(chatname, password);
}

void TextProcessor::command_leave(const std::string& command)
{
    std::string chatname;
    std::stringstream ss(command);
    ss >> chatname;

    if (chatname.empty()) {
        reportProblem(TOO_FEW_PARAMS);
        return;
    }


    auto chat = mContainer.getChat(chatname);
    if (!chat.has_value()) {
        reportProblem("No such chat found");
        return;
    }

    // TODO: "Are you sure?" message

    int id = chat->getId();
    auto leave_handler = [this, id] (CompletionError ec, ConstBuffer buffer) {

        if (ec == CompletionError::OK) {
            mContainer.removeChat(id);
            reportSuccess("Left");
        }
        else {
            reportProblem("Couldn't leave the chat", ec, buffer);
        }

    };

    mTasker.leaveChat(id, leave_handler);
}

void TextProcessor::command_create(const std::string& command)
{
    std::string chatname, password;
    std::stringstream ss(command);
    ss >> chatname;

    if (chatname.empty()) {
        reportProblem(TOO_FEW_PARAMS);
        return;
    }

    auto create_handler = [this] (CompletionError ec, ConstBuffer buffer) {
        if (ec == CompletionError::OK) {
            ChatInfo info;
            info.fillFromBuffer(buffer);
            mContainer.addChat(Chat(info));
        }
        else {
            reportProblem("Couldn't create chat", ec, buffer);
        }
    };

    if (password.empty())
        mTasker.createChat(chatname, create_handler);
    else
        mTasker.createChat(chatname, password, create_handler);

}

void TextProcessor::command_start(const std::string& command)
{
    std::string username;
    std::stringstream ss(command);
    ss >> username;

    if (username.empty()) {
        reportProblem(TOO_FEW_PARAMS);
        return;
    }

    bool fakeDispatch = false;

    auto user = mContainer.getUser(username);
    if (user.has_value())
        fakeDispatch = true;

    auto start_chat_handler = [this] (CompletionError ec, ConstBuffer buffer) {
        if (ec == CompletionError::OK) {
            reportSuccess("Direct chat with user created");
            ChatInfo info;
            info.fillFromBuffer(buffer);
            mContainer.addChat(Chat(info));
        }
        else {
            reportProblem("Couldn't create direct chat", ec, buffer);
        }
    };

    auto user_load_handler = [this, user, fakeDispatch, start_chat_handler] (CompletionError ec, ConstBuffer buffer) {
        User lUser;

        if (fakeDispatch) {
            lUser = user.value();
        }
        else if (ec == CompletionError::OK) {
            lUser.fillFromBuffer(buffer);
        }
        else {
            reportProblem("Couldn't find such user", ec, buffer);
            return;
        }

        mTasker.startChat(lUser.getId(), start_chat_handler);
    };

    mTasker.getUserData(username, user_load_handler);

}

void TextProcessor::command_msg(const std::string& command)
{
    std::string chatname;
    std::stringstream ss(command);
    ss >> chatname;

    if (chatname.empty()) {
        reportProblem(TOO_FEW_PARAMS);
        return;
    }

    auto chat = mContainer.getChat(chatname);
    if (chat.has_value()) {
        reportProblem("Couldn't find such chat");
        return;
    }

    runChat(*chat);
}

void TextProcessor::command_pmsg(const std::string& command)
{
    std::string username;
    std::stringstream ss(command);
    ss >> username;

    if (username.empty()) {
        reportProblem(Error::TOO_FEW_PARAMS);
        return;
    }

    auto user = mContainer.getUser(username);
    if (!user.has_value()) {
        reportProblem("You don't have any chats with such user");
        return;
    }

    auto chat = mContainer.getDirectChat(user->getId(), mUserId);
    if (!chat.has_value()) {
        reportProblem("Chat with this user doesn't exist");
        return;
    }

    runChat(*chat);
}

void TextProcessor::command_reconnect(const std::string& command)
{
// TODO:
}

void TextProcessor::command_disconnect(const std::string& command)
{

}

void TextProcessor::command_exit()
{
    mStopped = true;
}

void TextProcessor::command_help_a(const std::string& command)
{
    std::scoped_lock lock(mIOMutex);
    std::cout << mAuthorized_CommandSet.helpMessage();
}

void TextProcessor::command_help_c(const std::string& command)
{
    std::scoped_lock lock(mIOMutex);
    std::cout << mConnected_CommandSet.helpMessage();
}

void TextProcessor::command_help_d(const std::string& command)
{
    std::scoped_lock lock(mIOMutex);
    std::cout << mDisconnected_CommandSet.helpMessage();
}

void TextProcessor::_initDisconnected()
{
    mDisconnected_CommandSet.addCommand("help", [this](const std::string& command) {
        command_help_d(command);
    }, "Prints help info");

    mDisconnected_CommandSet.addCommand("reconnect", [this](const std::string& command) {
        command_reconnect(command);
    }, "Tries reconnect to server");

    mDisconnected_CommandSet.addCommand("exit", [this](const std::string& command) {
        command_exit();
    }, "Exits the program");
}

void TextProcessor::_initConnected()
{
    mConnected_CommandSet.addCommand("help", [this](const std::string& command) {
        command_help_c(command);
    }, "prints help");

    mConnected_CommandSet.addCommand("login", [this](const std::string& command) {
        command_login(command);
    });

    mConnected_CommandSet.addCommand("disconnect", [this](const std::string& command) {
        command_disconnect(command);
    }, "Disconnects from server // Does nothing");

    mConnected_CommandSet.addCommand("exit", [this](const std::string& command) {
        command_exit();
    }, "Closes the app");

    // TODO: command_restore_session

}

void TextProcessor::_initAuthorized()
{
    mAuthorized_CommandSet.addCommand("disconnect", [this](const std::string& command) {
        command_disconnect(command);
    }, "Disconnects from server // Does nothing");

    mAuthorized_CommandSet.addCommand("join", [this](const std::string& command) {
        command_join(command);
    }, "args: chat_name [, password]. Join chat.");

    mAuthorized_CommandSet.addCommand("leave", [this](const std::string& command) {
        command_leave(command);
    }, "args: chat_name. Leave chat.");

    mAuthorized_CommandSet.addCommand("create", [this](const std::string& command) {
        command_create(command);
    }, "args: chat_name [, password]. Create chat.");

    mAuthorized_CommandSet.addCommand("connect", [this](const std::string& command) {
        command_start(command);
    }, "args: username. Create direct chat with the user");

    mAuthorized_CommandSet.addCommand("msg", [this](const std::string& command) {
        command_msg(command);
    }, "args: chat_name. Opens group chat.");

    mAuthorized_CommandSet.addCommand("dmsg", [this](const std::string& command) {
        command_pmsg(command);
    }, "args: username. Opens direct chat.");

    mAuthorized_CommandSet.addCommand("help", [this](const std::string& command) {
        command_help_a(command);
    }, "Prints help");

    mAuthorized_CommandSet.addCommand("exit", [this](const std::string& /*command*/) {
        command_exit();
    }, "Closes the app");

}


