#ifndef MESSENGER_SERVER_MAIN_HPP
#define MESSENGER_SERVER_MAIN_HPP

#include <cassert>

#include <vector>
#include <string>
#include <stdexcept>

#include "Server.hpp"
#include "Context.hpp"

class Main
{
public:
    using pointer = std::unique_ptr<Main>;
    using reference = Main&;

public: // Singleton implementation

    // Creates instance of Main class
    static void init();

    // Create instance of Main class with specified config file
    static void init(const std::string& configFile);

    static reference getInstance();

public:
    void run();

    Context& getContext();
    Server& getServer();

private:
    Main();
    explicit Main(const std::string& configFile);

private:
    Context mContext;
    Server mServer;

    static pointer _instance;


};

#endif //MESSENGER_SERVER_MAIN_HPP
