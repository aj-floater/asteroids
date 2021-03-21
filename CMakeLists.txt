cmake_minimum_required(VERSION 3.0.0)
project(floater VERSION 0.1.0)

set (CMAKE_CXX_STANDARD 11)

find_package(glfw3 3.3 REQUIRED)
find_package(OpenAL REQUIRED)

include_directories(
    ./src 
    ./include
)

add_executable( 
    floater 
    
    ./src/main.cpp

    ./src/others/glad.c 
    ./src/others/stb_image.cpp

    ./src/game/player.cpp
    ./src/game/asteroid.cpp
    
    ./src/utility/coord.cpp
    ./src/utility/clamp.cpp
)

target_link_libraries( floater glfw )