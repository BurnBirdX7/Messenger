cmake_minimum_required(VERSION 3.17)
project(AsioApplication LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(WIN32)
    message("WIN32 detected")
    add_definitions(-D_WIN32_WINNT=0x0601) # Windows 7 target
endif()

if(MSVC)
    message("MSVC detected")
    add_definitions(-DBOOST_DATE_TIME_NO_LIB -DBOOST_REGEX_NO_LIB)
    #add_link_options(/NODEFAULTLIB:MSVCRT)
endif()

include(CMake/FindOpenSSL.cmake)

add_subdirectory(Commons)
include_directories(Commons)


add_subdirectory(Client)
add_subdirectory(Server)

target_link_libraries(Client PUBLIC Commons)
target_link_libraries(Server PUBLIC Commons)


target_link_libraries(Commons PUBLIC LibSSL LibCrypto)
target_link_libraries(Client PUBLIC LibSSL LibCrypto)
target_link_libraries(Server PUBLIC LibSSL LibCrypto)
