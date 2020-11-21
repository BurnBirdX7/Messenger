#ifndef ASIOAPPLICATION_MAIN_HPP
#define ASIOAPPLICATION_MAIN_HPP

#include <memory>
#include <vector>
#include <string>
#include <optional>

#include "Client.hpp"

class Main
{
public:

    static int main(const std::vector<std::string>& args);

private:

    static std::optional<Client> mClient;

};


#endif //ASIOAPPLICATION_MAIN_HPP
