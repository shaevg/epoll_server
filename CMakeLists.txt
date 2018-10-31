cmake_minimum_required(VERSION 2.8)

project(epoll_server)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

set(IO_MODULE_SOURCES io_module.cpp io_module.h)
set(MESSAGE_SOURCES message.cpp message.h)

find_package(Threads REQUIRED)
add_library(io_module_lib STATIC ${IO_MODULE_SOURCES})
add_library(message_lib STATIC ${MESSAGE_SOURCES})

add_executable(${PROJECT_NAME} "main.cpp")

target_link_libraries(${PROJECT_NAME} io_module_lib)
target_link_libraries(${PROJECT_NAME} message_lib)
target_link_libraries(${PROJECT_NAME} Threads::Threads)
