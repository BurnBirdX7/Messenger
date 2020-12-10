#include "Main.hpp"

std::unique_ptr<Main> Main::_instance{nullptr};

int main(int argc, char* argv[])
{
    size_t arg_count = static_cast<size_t>(argc) - 1; // ignore first argument
    std::vector<std::string> vec_args(arg_count);

    for (size_t i = 0; i < arg_count; ++i)
        vec_args[i + 1] = std::string(argv[i + 1]);

    // Ignore args // TODO: process args
    Main& main = Main::getInstance();
    return main.run();
}

void Main::init()
{
    assert(_instance && "Instance of Main class already exists");

    auto rawPtr = new Main();
    pointer unique(rawPtr);
    _instance.swap(unique);

    if (unique)
        throw std::runtime_error("Pointer swap error");

}

void Main::init(const std::string& configFile)
{
    assert(_instance && "Instance of Main class already exists");

    auto rawPtr = new Main(configFile);
    pointer unique(rawPtr);
    _instance.swap(unique);

    if (unique)
        throw std::runtime_error("Pointer swap error");

}

Main& Main::getInstance()
{
    if (!_instance)
        init();

    return *_instance;
}

int Main::run()
{
    mClient.start( [] (Commons::Network::Task::ErrorCode ec, Commons::Data::ConstBuffer buffer) {

        if (ec != Commons::Network::Task::ErrorCode::OK)
            return -1;



    } );

}

Client& Main::getClient()
{
    return Main::mClient;
}

Context& Main::getContext()
{
    return Main::mContext;
}

Main::Main()
        : mContext()
        , mClient(mContext)
{}

Main::Main(const std::string& configFile)
        : mContext(configFile)
        , mClient(mContext)
{}
