#include <locale>

#include "CommandSet.hpp"

CommandSet::CommandSet()
    : mCommands()
{}

void CommandSet::addCommand(const std::string& commandName,
                            const CommandSet::CommandHandler& handler,
                            const std::string& description)
{
    assert( (mCommands.find(commandName) == mCommands.end()) && "Command with this mName already added to the list" );

    mCommands[commandName] = Command{handler, description};
}

int CommandSet::execute(const std::string& command)
{
    size_t len = command.length();
    std::string commandName;

    size_t i;
    for (i = 0; i < len && !std::isspace(command[i], std::locale()) ; ++i)
        commandName += command[i];

    // command line without the command itself
    std::string commandLine(command.c_str() + i);

    auto cmdIt = mCommands.find(commandName);

    if (cmdIt == mCommands.end())
        return NO_SUCH_COMMAND;

    std::invoke(cmdIt->second.first, commandLine);

    return OK;
}

std::string CommandSet::helpMessage() const
{
    std::string help;
    for (const auto& entry : mCommands)
        help += (entry.first + "\t - ") + (entry.second.second + '\n');

    return help;
}
