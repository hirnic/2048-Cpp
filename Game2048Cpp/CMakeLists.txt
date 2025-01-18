cmake_minimum_required(VERSION 3.30)
project(Game2048Cpp)

set(CMAKE_CXX_STANDARD 20)

# Specify Unicode entry point to avoid looking for 'WinMain'
add_executable(
        Game2048Cpp
        main.cpp
        Mechanics.h
        Mechanics.cpp
        Animations.h
        Animations.cpp
)

# Define the entry point as wWinMain for Unicode support
target_compile_definitions(Game2048Cpp PRIVATE UNICODE)

# Explicitly set the entry point for the Windows application to use wWinMain
set_target_properties(Game2048Cpp PROPERTIES LINK_FLAGS "-municode")

# Link against necessary libraries
target_link_libraries(Game2048Cpp user32 kernel32 gdi32)