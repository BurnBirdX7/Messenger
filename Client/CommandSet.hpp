#ifndef MESSENGER_COMMANDSET_HPP
#define MESSENGER_COMMANDSET_HPP

#include <cassert>
#include <functional>
#include <map>
#include <string>

#include "Tasker.hpp"

class CommandSet
{
public:
    using CommandHandler = std::function<void (const std::string&)>;

    using Map = std::map<std::string /* cmd mName */, CommandHandler /* handler */>;

    // Errors
    enum Error {
        OK = 0,
        NO_SUCH_COMMAND = -1,
        WRONG_ARGUMENTS = -2
    };

public:
    CommandSet();
    void addCommand(const std::string& commandName, const CommandHandler& handler);
    int execute(const std::string& command);

private:
    Map mCommands;

};


#endif //MESSENGER_COMMANDSET_HPP
