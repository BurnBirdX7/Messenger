#ifndef MESSENGER_COMMANDSET_HPP
#define MESSENGER_COMMANDSET_HPP

#include <cassert>
#include <functional>
#include <map>
#include <string>
#include <string_view>

#include "Tasker.hpp"

class CommandSet
{
public:
    using CommandHandler = std::function<void (const std::string_view&)>;

    using Map = std::map<std::string /* cmd name */, CommandHandler /* handler */>;

    // Errors
    enum Error {
        OK = 0,
        NO_SUCH_COMMAND = -1,
        WRONG_ARGUMENTS = -2
    };

public:
    explicit CommandSet(Tasker&);

    void addCommand(const std::string& commandName, const CommandHandler& handler);

    int execute(const std::string& command);

private:
    Tasker& mTasker;
    Map mCommands;

};


#endif //MESSENGER_COMMANDSET_HPP
