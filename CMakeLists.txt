cmake_minimum_required(VERSION 3.23)
project(Automat)

set(CMAKE_CXX_STANDARD 17)


find_package(SDL2 REQUIRED)


include_directories(${SDL2_INCLUDE_DIRS})

add_executable(Automat main.cpp classes/Node.hpp classes/RBTree.hpp Test.cpp funcs.cpp funcs.h)


target_link_libraries(Automat  ${SDL2_LIBRARIES} SDL2_ttf SDL2_image)