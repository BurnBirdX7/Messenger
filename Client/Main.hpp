#ifndef ASIOAPPLICATION_MAIN_HPP
#define ASIOAPPLICATION_MAIN_HPP

#include <cassert>

#include <memory>
#include <string>
#include <stdexcept>

#include "Client.hpp"  // Client class
#include "Context.hpp" // Context class

/*
 * Main class
 * Singleton
 */
class Main
{
public:
    using pointer = std::unique_ptr<Main>;
    using reference = Main&;

public: // Singleton implementation
    // Creates instance with default config file
    static void init();

    // Creates instance with specified config file
    static void init(const std::string& configFile);

    // Returns an already existing instance
    // If there's no such creates new with default parameters
    static reference getInstance();

public:
    int run();

    Client& getClient();
    Context& getContext();

private:
    Main();
    explicit Main(const std::string& configFile);

private:
    Context mContext;
    Client mClient;

    static pointer _instance;

};


#endif //ASIOAPPLICATION_MAIN_HPP
