
#include "Main.hpp"

int main(int argc, char* argv[])
{
    std::vector<std::string> vec_args(argc);

    for (size_t i = 0; i < argc; ++i)
        vec_args[i] = std::string(argv[i]);

    return Main::main(vec_args);
}


int Main::main(const std::vector<std::string>& args)
{

    return 0;
}




