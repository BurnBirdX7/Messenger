#include "Main.hpp"

Main::pointer Main::_instance{nullptr};

int main(int argc, char* argv[])
{
    size_t arg_count = argc - 1; // Ignore first argument

    std::vector<std::string> vec_args(arg_count);

    for (size_t i = 0; i < arg_count; ++i)
        vec_args[i + 1] = std::string(argv[i + 1]);

    Main& main = Main::getInstance();
    main.run();

    return 0;
}

void Main::init()
{
    assert(!_instance && "Instance of Main class already exists");

    auto rawPtr = new Main();
    pointer unique(rawPtr);
    _instance.swap(unique);

    if (unique)
        throw std::runtime_error("Pointer swap error");

}

void Main::init(const std::string& configFile)
{
    assert(!_instance && "Instance of Main class already exists");

    auto rawPtr = new Main(configFile);
    pointer unique(rawPtr);
    _instance.swap(unique);

    if (unique)
        throw std::runtime_error("Pointer swap error");
}

Main::reference Main::getInstance()
{
    if (!_instance)
        init();

    return *_instance;
}

void Main::run()
{
    auto io_thread = std::move(mContext.getIoThread());
    mServer->start();
    io_thread.join();
}

Context& Main::getContext()
{
    return mContext;
}

Server& Main::getServer()
{
    return *mServer;
}

Main::Main()
    : mContext()
    , mServer(new Server(mContext))
{}

Main::Main(const std::string& configFile)
    : mContext(configFile)
    , mServer(new Server(mContext))
{}
