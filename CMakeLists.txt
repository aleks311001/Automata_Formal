cmake_minimum_required(VERSION 3.17)
project(Formal)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Werror")

add_executable(Formal main.cpp NKA.cpp NKA.h)

add_executable(Test Tests.cpp NKA.h NKA.cpp)
target_link_libraries(Test gtest)
