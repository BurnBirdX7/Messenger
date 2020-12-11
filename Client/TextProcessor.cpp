#include "TextProcessor.hpp"

#include <utility>

TextProcessor::TextProcessor(Client& client)
    : mTasker(client)
    , mContainer(mTasker)
    , mCurrentCommandSet(nullptr)
    , mActiveChat(nullptr)
    , mStopped(false)
{
    if (client.isConnected())
        mCurrentCommandSet = &mConnected;
    else
        mCurrentCommandSet = &mDisconnected;

    setDisconnected();
    setConnected();
    setAuthorized();
}

void TextProcessor::run()
{

    while (!mStopped) {

        std::string line;
        {
            std::scoped_lock lock(mIOMutex);
            std::getline(std::cin, line, '\n');
        }
        if (mActiveChat) {
            if (line == "quit") {
                mActiveChat = nullptr;
                continue;
            }
        }
        else {
            mCurrentCommandSet->execute(line);
        }

    }

}

/* GENERAL ACTIONS: */

void TextProcessor::authorize(ConstBuffer buffer)
{
    BufferDecomposer decomposer(buffer);
    int userId;
    decomposer
        .extract(mSessionId)
        .extract(mSessionHash)
        .extract(userId);

    mContainer.setCurrentUser(userId);
    mTasker.authorizeClient();
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
    std::cout << msg << ": (Code: " << ec << ") ";
    std::cout << decomposer.get<std::string>() << std::endl;
}

void TextProcessor::reportSuccess(const std::string& msg)
{
    std::scoped_lock lock(mIOMutex);
    std::cout << "Success: " << msg << std::endl;
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



void TextProcessor::chat_join(const ChatPtr& chat, const std::string& password)
{
    if (chat->hasPassword && password.empty()) {
        reportProblem("The chat requires a password");
        return;
    }

    auto handler = [this, chat] (CompletionError ec, ConstBuffer buffer) {
        if (ec == CompletionError::OK) {
            reportSuccess("Joined to chat");
            mContainer.addUserChat(chat);
        }
        else {
            reportProblem("Join chat", ec, buffer);
        }
    };

    if (chat->hasPassword) {
        mTasker.joinChat(chat->id, password, handler);
    }
    else {
        mTasker.joinChat(chat->id, handler);
    }

}

void TextProcessor::leave_chat(const ChatPtr& chat)
{
    mTasker.leaveChat(chat->id, [=] (CompletionError ec, ConstBuffer buffer) {

        const char* str = "Leave chat";
        if (ec == CompletionError::OK) {
            mContainer.removeUserChat(chat);
            reportSuccess(str);
        }
        else {
            reportProblem(str, ec, buffer);
        }

    });
}

void TextProcessor::create_chat(const std::string& name)
{

    mTasker.createChat(name, [this, name] (CompletionError ec, ConstBuffer buffer) {

        const char* str = "Create chat";
        if (ec == CompletionError::OK) {
            reportSuccess(str);
            mContainer.loadChat(name, [this] (bool ok, const ChatPtr& chat) {

                if (ok) {
                    mContainer.loadMessages(chat->id, 0, nullptr);
                }
                else {
                    reportProblem("Can't load new chat");
                }

            });
        }
        else {
            reportProblem(str, ec, buffer);
        }

    });

}

void TextProcessor::create_chat(const std::string& name, const std::string& password)
{

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
            authorize(buffer);
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

    auto chat = mContainer.getChat(chatname);
    if (!chat) { // if chat == nullptr
        mContainer.loadChat(chatname, [this, password] (bool ok, const ChatPtr& ptr) {
            if (ok)
                chat_join(ptr, password);
            else
                reportProblem("Couldn't get such chat");
        });
    }
    else
        chat_join(chat, password);

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
    if (!chat) { // if chat == nullptr

        mContainer.loadChat(chatname, [this](bool ok, const ChatPtr& ptr) {
            if (ok)
                leave_chat(ptr);
            else
                reportProblem(CANT_GET_THE_CHAT);
        });

    }
    else {
        leave_chat(chat);
    }
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

    if (password.empty())
        create_chat(chatname);
    else
        create_chat(chatname, password);

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

    if (!chat) {

        mContainer.loadChat(chatname, [this] (bool ok, ChatPtr ptr) {
            if (ok) {
                mActiveChat = std::move(ptr);
            }
            else {
                reportProblem(CANT_GET_THE_CHAT);
            }
        });

    }
    else {
        mActiveChat = chat;
    }

}

void TextProcessor::command_reconnect(const std::string& command)
{
// TODO:
}

void TextProcessor::command_disconnect(const std::string& command)
{

}

void TextProcessor::command_exit(const std::string& command)
{
    mStopped = true;
}

void TextProcessor::command_help_d(const std::string& command)
{

}

void TextProcessor::command_help_c(const std::string& command)
{

}

void TextProcessor::command_help_a(const std::string& command)
{

}

void TextProcessor::setDisconnected()
{
    mDisconnected.addCommand("help", [this] (const std::string& command) {
        command_help_d(command);
    });

    mDisconnected.addCommand("reconnect", [this] (const std::string& command) {
        command_reconnect(command);
    });
}

void TextProcessor::setConnected()
{
    mDisconnected.addCommand("help", [this] (const std::string& command) {
        command_help_c(command);
    });

    mConnected.addCommand("login", [this](const std::string& command) {
        command_login(command);
    });

    mConnected.addCommand("disconnect", [this](const std::string& command) {
        command_disconnect(command);
    });

    // TODO: command_restore_session

}

void TextProcessor::setAuthorized()
{
    mConnected.addCommand("disconnect", [this](const std::string& command) {
        command_disconnect(command);
    });

    mAuthorized.addCommand("join", [this] (const std::string& command) {
        command_join(command);
    });

    mAuthorized.addCommand("leave", [this] (const std::string& command) {
        command_leave(command);
    });

    mAuthorized.addCommand("create", [this] (const std::string& command) {
        command_create(command);
    });

    mAuthorized.addCommand("msg", [this] (const std::string& command) {
        command_msg(command);
    });

    mAuthorized.addCommand("help", [this] (const std::string& command) {
        command_help_a(command);
    });

}


