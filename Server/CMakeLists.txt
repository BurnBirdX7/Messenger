cmake_minimum_required(VERSION 3.17)

project(ServerApplication VERSION 1.0 LANGUAGES CXX)

add_executable(
        Server

        Main.cpp
        Main.hpp
        Server.cpp
        Server.hpp
        Connection.cpp
        Connection.hpp
        Context.cpp
        Context.hpp)

set(DATABASE_SRC
        DbException.h
        Database/DbPool.h
        Database/DbPool.cpp
        Database/AbstractDao.h
        Database/ChatDao.h
        Database/ChatDao.cpp
        Database/UserDao.h
        Database/UserDao.cpp
        Database/MessageDao.h
        Database/MessageDao.cpp
        Database/SessionDao.h
        Database/SessionDao.cpp)

add_executable(${DATABASE_SRC})
target_link_libraries(Server PUBLIC pqxx libpq wsock32 ws2_32)
