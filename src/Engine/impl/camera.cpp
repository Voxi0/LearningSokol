#include"../Engine.hpp"

// Constructor - Initialize The Camera
sgl::Camera::Camera(const glm::vec3 &position, const glm::vec3 &front, const float &moveSpeed, const float &lookSpeed, const float &fov, const float &minFOV, const float &maxFOV) {
    // Position and Front Vector
    this->position = position;
    this->front = front;

    // Move and Look Speed (Mouse Sensitivity)
    this->moveSpeed = moveSpeed;
    this->lookSpeed = lookSpeed;

    // FOV
    this->fov = fov;
    this->minFOV = minFOV;
    this->maxFOV = maxFOV;
}

// Process Keyboard Input
void sgl::Camera::processKeyboard(GLFWwindow* win, const float &deltaTime) {
    // Move Around
    if(glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) {
        this->position += this->front * (this->moveSpeed * deltaTime);
    } if(glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) {
        this->position -= this->front * (this->moveSpeed * deltaTime);
    } if(glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) {
        this->position -= glm::normalize(glm::cross(this->front, this->up)) * (this->moveSpeed * deltaTime);
    } if(glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) {
        this->position += glm::normalize(glm::cross(this->front, this->up)) * (this->moveSpeed * deltaTime);
    }
}

// Process Mouse Movement (Look Around) / Scroll (Zoom In/Out)
void sgl::Camera::processMouse(const double &mousePosX, const double &mousePosY) {
    // Check if The Mouse Cursor Entered The Window For The First Time
    if(this->firstMouse) {
        // Update Mouse Last Position
        this->mouseLastX = static_cast<float>(mousePosX);
        this->mouseLastY = static_cast<float>(mousePosY);

        // Mouse Cursor Has Now Entered The Window For The First Time
        this->firstMouse = false;
    }

    // Calculate Mouse Position Offset
    float xOffset {static_cast<float>(mousePosX) - this->mouseLastX};
    float yOffset {this->mouseLastY - static_cast<float>(mousePosY)};

    // Update Mouse Last Position
    this->mouseLastX = static_cast<float>(mousePosX);
    this->mouseLastY = static_cast<float>(mousePosY);

    // Apply Look Speed (Mouse Sensitivity) to Mouse Position Offset
    xOffset *= this->lookSpeed;
    yOffset *= this->lookSpeed;

    // Update Camera Yaw and Pitch
    this->yaw += xOffset;
    this->pitch += yOffset;

    // Constrain Camera Pitch So The Camera Cannot Rotate 360 Degrees Vertically
    if(this->pitch < -89.0f) {
        this->pitch = -89.0f;
    } if(this->pitch > 89.0f) {
        this->pitch = 89.0f;
    }

    // Calculate and Set New Camera Direction
    glm::vec3 dir;
    dir.x = glm::cos(glm::radians(this->yaw)) * glm::cos(glm::radians(this->pitch));
    dir.y = glm::sin(glm::radians(this->pitch));
    dir.z = glm::sin(glm::radians(this->yaw)) * glm::cos(glm::radians(this->pitch));
    this->front = glm::normalize(dir);
}
void sgl::Camera::processScroll(const double &scrollPosY) {
    // Zoom In/Out
    this->fov -= static_cast<float>(scrollPosY);
    if(this->fov < this->minFOV) {
        this->fov = this->minFOV;
    } if(this->fov > this->maxFOV) {
        this->fov = this->maxFOV;
    }
}

// Getters
glm::mat4 sgl::Camera::getViewMat(void) {return glm::lookAt(this->position, this->position + this->front, this->up);}
glm::vec3 sgl::Camera::getPos(void) {return this->position;}
glm::vec3 sgl::Camera::getFront(void) {return this->front;}
float sgl::Camera::getFOV(void) {return this->fov;}
