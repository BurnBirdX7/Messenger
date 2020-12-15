cmake_minimum_required(VERSION 3.17)

project(ClientApplication VERSION 1.0 LANGUAGES CXX)

add_executable(
        Client

        Main.cpp
        Main.hpp
        Client.cpp
        Client.hpp
        Context.cpp
        Context.hpp
        Tasker.cpp
        Tasker.hpp
        ClientErrorCategory.cpp
        ClientErrorCategory.hpp
        TextProcessor.cpp
        TextProcessor.hpp
        CommandSet.cpp
        CommandSet.hpp
        Container2.cpp
        Container2.hpp)

