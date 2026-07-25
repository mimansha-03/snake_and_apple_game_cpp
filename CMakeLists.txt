cmake_minimum_required(VERSION 3.16)
project(SnakeAndApple CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

include(FetchContent)

set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
set(SUPPORT_MODULE_RTEXT OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG 5.5
    GIT_SHALLOW TRUE
)

FetchContent_MakeAvailable(raylib)

add_executable(snake_game
    src/main.cpp
    src/Game.cpp
    src/Snake.cpp
    src/Apple.cpp
    src/Renderer.cpp
    src/Menu.cpp
    src/Animation.cpp
)

target_include_directories(snake_game PRIVATE src)
target_link_libraries(snake_game PRIVATE raylib)

if(MSVC)
    target_compile_options(snake_game PRIVATE /W4)
else()
    target_compile_options(snake_game PRIVATE -Wall -Wextra -O2)
endif()

if(APPLE)
    target_link_libraries(snake_game PRIVATE
        "-framework IOKit"
        "-framework Cocoa"
        "-framework OpenGL"
    )
elseif(UNIX)
    target_link_libraries(snake_game PRIVATE
        m pthread dl rt X11
    )
endif()