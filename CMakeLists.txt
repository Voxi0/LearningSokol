# Set Minimum Version of CMake Required to Generate Project Build Files
cmake_minimum_required(VERSION 3.28)

# Create Project
project(learningSokol LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS_DEBUG "-Wall -Werror -Wextra -Wpedantic")
set(CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG")
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Add Subdirectories to Compile - Must Contain A CMakeLists.txt File
add_subdirectory("3rdparty/glfw")

# Set Proeject Include Directories
include_directories(${PROJECT_NAME}
    "3rdparty/sokol/"
    "3rdparty/glfw/include/"
    "3rdparty/glm/"
    "3rdparty/stb/"
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
    "src/Engine/impl/texture.cpp"
    "src/Engine/impl/camera.cpp"
)

# Project Linkers
target_link_libraries(${PROJECT_NAME}
    glfw GL
)
