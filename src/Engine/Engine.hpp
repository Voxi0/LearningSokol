#pragma once

// Standard Headers
#include<iostream>
#include<cstdlib>

// Sokol
#include<sokol_gfx.h>
#include<sokol_log.h>

// GLFW
#define GLFW_INCLUDE_NONE
#include<GLFW/glfw3.h>

// OpenGL Mathematics - GLM
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

// STB Image
#include<stb_image.h>

namespace sgl {
    // Texture Class
    class Texture {
        public:
            // Constructor - Load Texture Image File and Create Texture Object
            Texture(sg_image_desc &imgDesc, const std::string &texImgFilePath);

            // Getters
            sg_image getImage(void);
        private:
            // Texture Object
            sg_image texture;
    };

    // Camera Class
    class Camera {
        public:
            // Constructor - Initialize The Camera
            Camera(const glm::vec3 &position, const glm::vec3 &front, const float &moveSpeed, const float &lookSpeed, const float &fov, const float &minFOV, const float &maxFOV);

            // Process Keyboard Input
            void processKeyboard(GLFWwindow* win, const float &deltaTime);

            // Process Mouse Movement (Look Around) / Scroll (Zoom In/Out)
            void processMouse(const double &scrollPosX, const double &scrollPosY);
            void processScroll(const double &scrollPosY);

            // Getters
            glm::mat4 getViewMat(void);
            glm::vec3 getPos(void);
            glm::vec3 getFront(void);
            float getFOV(void);

            // Setters
            void setFirstMouse(const bool &value);
        private:
            // Position, Front and Up Vector
            glm::vec3 position, front, up {0.0f, 1.0f, 0.0f};

            // Move and Look Speed (Mouse Sensitivity)
            float moveSpeed, lookSpeed;

            // FOV
            float fov, minFOV, maxFOV;

            // Mouse Entered The Window For The First Time
            bool firstMouse {true};

            // Mouse Last Position
            float mouseLastX, mouseLastY;

            // Yaw and Pitch
            float yaw {-90.0f}, pitch;
    };
};
