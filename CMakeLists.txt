cmake_minimum_required(VERSION 3.16)
project(SnakeAndApple CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Release)
endif()

include(FetchContent)

# raylib is fetched and built from source automatically - no manual install step needed.
set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(BUILD_GAMES OFF CACHE BOOL "" FORCE)
set(SUPPORT_MODULE_RTEXT OFF CACHE BOOL "" FORCE)

FetchContent_Declare(
    raylib
    GIT_REPOSITORY https://github.com/raysan5/raylib.git
    GIT_TAG        5.5
    GIT_SHALLOW    TRUE
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

# raylib on Linux/macOS needs a couple of extra system frameworks/libs.
if(APPLE)
    target_link_libraries(snake_game PRIVATE "-framework IOKit" "-framework Cocoa" "-framework OpenGL")
elseif(UNIX AND NOT APPLE)
    target_link_libraries(snake_game PRIVATE m pthread dl rt X11)
endif()
