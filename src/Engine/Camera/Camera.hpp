#pragma once

// GLFW
#include<GLFW/glfw3.h>

// OpenGL Mathematics
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

// Camera Class
class Camera {
    public:
        // Constructor - Initialize The Camera
        Camera(const glm::vec3 position, const float moveSpeed, const float lookSpeed, const float fov, const float minFOV, const float maxFOV);

        // Handle Keyboard Input - Camera Movement
        void handleKeyboardInput(GLFWwindow* win, const float deltaTime);

        // Handle Mouse / Scroll Input - Look Around / Zoom In and Out
        void handleMouseInput(const double mousePosX, const double mousePosY);
        void handleScrollInput(const double scrollPosY);

        // Getters
        glm::vec3 getPos(void);
        glm::vec3 getFront(void);
        glm::mat4 getViewMatrix(void);
        float getFOV(void);
    private:
        // Position, Direction and Up Vector
        glm::vec3 position, front {0.0f, 0.0f, -1.0f}, up {0.0f, 1.0f, 0.0f};

        // Move and Look Speed
        float moveSpeed, lookSpeed;

        // FOV
        float fov, minFOV, maxFOV;

        // Mouse Entered/Focused on The Window For The First Time
        bool firstMouse {true};

        // Last Mouse X and Y Position
        float mouseLastX, mouseLastY;

        // Yaw (Left and Right) and Pitch (Up and Down)
        float yaw {-90.0f}, pitch;
};
