#ifndef MESSENGER_TEXTPROCESSOR_HPP
#define MESSENGER_TEXTPROCESSOR_HPP

#include "CommandSet.hpp"

class TextProcessor
{
public:


private:
    CommandSet* mCurrentCommandSet;

    // Set of commands for state "Disconnected"
    CommandSet M_DISCONNECTED;

    // Set of commands for state "Connected"
    CommandSet M_CONNECTED;

    // Set of commands for state "Authorized"
    CommandSet M_AUTHORIZED;

};


#endif //MESSENGER_TEXTPROCESSOR_HPP
