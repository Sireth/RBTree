cmake_minimum_required(VERSION 3.22)
project(RBTree)

set(CMAKE_CXX_STANDARD 17)


add_executable(RBTree main.cpp classes/Node.hpp classes/RBTree.hpp Test.cpp funcs.cpp funcs.h)

#SDL2 --------------------
add_subdirectory(external/SDL2)
target_include_directories(RBTree PRIVATE external/SDL2)
#-------------------------


#SDL2_image --------------
add_subdirectory(external/SDL2_image)
target_include_directories(RBTree PRIVATE external/SDL2_image)
#-------------------------



#freetype ----------------
add_subdirectory(external/freetype)
#-------------------------



#SDL2_ttf ----------------
add_subdirectory(external/SDL2_ttf)
target_include_directories(RBTree PRIVATE external/SDL2_ttf)
#-------------------------






target_link_libraries(RBTree  SDL2::SDL2 SDL2_image::SDL2_image SDL2_ttf::SDL2_ttf)