# Minimum Version of CMake Required to Generate Project Build Files
cmake_minimum_required(VERSION 3.28)

# Create Project and Set Programming Standards
project(learningSokol LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Werror -Wextra -ggdb")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

# Add 3rdparty Dependencies That Needs to be Compiled - Their Needs to be A CMakeLists.txt File in The Given Directory
add_subdirectory("3rdparty/glfw")

# Set Project Include Directories
include_directories(${PROJECT_NAME}
    "3rdparty/glfw/include/"
    "3rdparty/glm/"
    "3rdparty/sokol/"
)

# Set Project Link Directories
link_directories(${PROJECT_NAME}
    "build/3rdparty/glfw/src/"
)

# Link Source Files to Project
add_executable(${PROJECT_NAME}
    # Main Source File
    "src/main.cpp"

    # Custom Engine Source
    "src/Engine/Camera/camera.cpp"
)

# Project Linkers
target_link_libraries(${PROJECT_NAME}
    glfw GL
)
