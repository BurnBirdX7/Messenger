cmake_minimum_required(VERSION 3.17)

project(ServerApplication VERSION 1.0 LANGUAGES CXX)

add_executable(
        Server

        Main.cpp
        Main.hpp
        Server.cpp
        Server.hpp
        Connection.cpp
        Connection.hpp)
