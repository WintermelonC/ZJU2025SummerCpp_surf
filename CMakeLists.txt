cmake_minimum_required(VERSION 3.19)
project(surf)

set(CMAKE_CXX_STANDARD 11)

find_package(SFML 3.0 COMPONENTS System Window Graphics Audio REQUIRED)

add_subdirectory(code)
