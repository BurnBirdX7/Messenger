#include "Main.hpp"

std::unique_ptr<Main> Main::_instance{nullptr};

int main(int argc, char* argv[])
{
    try {
        size_t arg_count = static_cast<size_t>(argc) - 1; // ignore first argument
        std::vector<std::string> vec_args(arg_count);

        for (size_t i = 0; i < arg_count; ++i)
            vec_args[i + 1] = std::string(argv[i + 1]);

        // Ignore args // TODO: process args
        Main& main = Main::getInstance();
        return main.run();
    }
    catch (const boost::property_tree::file_parser_error& error) {
        std::cerr << error.message() << " ~ " << error.filename() << std::endl;
    }
    catch (const boost::system::system_error& error) {

        std::cerr << error.what() << std::endl;

    }
}

void Main::init()
{
    assert(!_instance && "Instance of Main class already exists");

    _instance.reset(new Main());

}

void Main::init(const std::string& configFile)
{
    assert(!_instance && "Instance of Main class already exists");

    _instance.reset(new Main(configFile));
}

Main& Main::getInstance()
{
    if (!_instance)
        init();

    return *_instance;
}

int Main::run()
{
    mClient.setNotificationHandler([this](uint8_t header, ConstBuffer buffer) {
        mProcessor.clientNotificationHandler(header, buffer);
    });

    mClient.setStateHandler([this](Client::State state) {
        mProcessor.clientStateHandler(state);
    });

    auto io_thread = std::move(mContext.getIoThread());

    mProcessor.run();

    io_thread.join();

    return 0;
}

Client& Main::getClient()
{
    return mClient;
}

Context& Main::getContext()
{
    return mContext;
}

Main::Main()
        : mContext()
        , mClient(mContext)
        , mProcessor(mClient)
{}

Main::Main(const std::string& configFile)
        : mContext(configFile)
        , mClient(mContext)
        , mProcessor(mClient)
{}
