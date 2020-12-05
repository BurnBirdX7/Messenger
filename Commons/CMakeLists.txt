cmake_minimum_required(VERSION 3.17)

project(CommonComponents VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)


# Boost
set(BOOST_REQUIRED_VERSION 1.74.0)
find_package(Boost ${BOOST_REQUIRED_VERSION} REQUIRED)
if (${Boost_FOUND})
    message("Boost - found (Boost ${Boost_VERSION_STRING})")
    include_directories(${Boost_INCLUDE_DIRS})
else()
    message(WARNING "Boost - not found")
endif ()
# ! Boost

cmake_minimum_required(VERSION 3.17)

set(COMMONS_NETWORK_SRC
        Network.hpp
        Network/SslConnection.hpp
        Network/SslConnection.cpp
        Network/Message.hpp
        Network/Message.cpp
        Network/MessageRepresentation.hpp
        Network/MessageRepresentation.cpp
        Network/Task.cpp
        Network/Task.hpp
        Network/TaskManager.cpp
        Network/TaskManager.hpp
        Network/Purpose.hpp
        Network/NetworkTypes.hpp
        Network/IMessage.hpp
        Network/NetworkErrorCategory.hpp
        Network/NetworkErrorCategory.cpp)

set(COMMONS_SYSTEM_SRC
        System.hpp
        System/Error.hpp
        System/Error.cpp
        System/BaseContext.cpp
        System/BaseContext.hpp
        System/ErrorCode.cpp
        System/ErrorCode.hpp
        System/ErrorCategory.hpp
        System/GenericErrorCategory.hpp
        System/Exception.hpp
        System/GenericErrorCategory.cpp)

set(COMMONS_DATA_SRC
        Data/ChatMessage.hpp
        Data/ISendable.hpp
        Data/PrimitiveTypes.hpp
        Data/ChatMessage.cpp
        Data/IReceivable.hpp)

set(COMMONS_INCLUDE
        ${CMAKE_CURRENT_SOURCE_DIR})

add_library(
        Commons STATIC

        ${COMMONS_NETWORK_SRC}
        ${COMMONS_SYSTEM_SRC}
        ${COMMONS_DATA_SRC}
        )

target_include_directories(Commons PUBLIC ${Boost_INCLUDE_DIRS})


