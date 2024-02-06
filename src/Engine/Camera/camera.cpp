#include"Camera.hpp"

// Constructor - Initialize The Camera
Camera::Camera(const glm::vec3 position, const float moveSpeed, const float lookSpeed, const float fov, const float minFOV, const float maxFOV) {
    // Set Position
    this->position = position;

    // Set Move Speed and Look Speed (Mouse Sensitivity)
    this->moveSpeed = moveSpeed;
    this->lookSpeed = lookSpeed;

    // Set FOV
    this->fov = fov;
    this->minFOV = minFOV;
    this->maxFOV = maxFOV;
}

// Handle Keyboard Input
void Camera::handleKeyboardInput(GLFWwindow* win, const float deltaTime) {
    float speed = this->moveSpeed * deltaTime;
    if(glfwGetKey(win, GLFW_KEY_W) == GLFW_PRESS) {
        this->position += this->front * speed;
    } if(glfwGetKey(win, GLFW_KEY_S) == GLFW_PRESS) {
        this->position -= this->front * speed;
    } if(glfwGetKey(win, GLFW_KEY_A) == GLFW_PRESS) {
        this->position -= glm::normalize(glm::cross(this->front, this->up)) * speed;
    } if(glfwGetKey(win, GLFW_KEY_D) == GLFW_PRESS) {
        this->position += glm::normalize(glm::cross(this->front, this->up)) * speed;
    }
}

// Handle Mouse/Scroll Input - Look Around / Zoom In and Out
void Camera::handleMouseInput(const double mousePosX, const double mousePosY) {
    // Check if Mouse Focused on/Entered The Window For The First Time
    if(this->firstMouse) {
        // Set Mouse Last X and Y Position
        this->mouseLastX = mousePosX;
        this->mouseLastY = mousePosY;

        // The Mouse Has Focused on/Entered The Window Now
        this->firstMouse = false;
    }

    // Calculate Mouse Position Offset
    double xOffset {mousePosX - this->mouseLastX};
    double yOffset {this->mouseLastY - mousePosY};

    // Update Mouse Last X and Y Position
    this->mouseLastX = mousePosX;
    this->mouseLastY = mousePosY;

    // Apply Look Speed to Offset
    xOffset *= this->lookSpeed;
    yOffset *= this->lookSpeed;

    // Set Camera Yaw and Pitch Values
    this->yaw += xOffset;
    this->pitch += yOffset;

    // Constrain Camera Pitch Value - Stops The Camera From Rotating 360 Degrees
    if(this->pitch < -89.0f) {
        this->pitch = -89.0f;
    } if(this->pitch > 89.0f) {
        this->pitch = 89.0f;
    }

    // Calculate and Set New Camera Direction
    glm::vec3 direction;
    direction.x = glm::cos(glm::radians(this->yaw)) * glm::cos(glm::radians(this->pitch));
    direction.y = glm::sin(glm::radians(this->pitch));
    direction.z = glm::sin(glm::radians(this->yaw)) * glm::cos(glm::radians(this->pitch));
    this->front = glm::normalize(direction);
}
void Camera::handleScrollInput(const double scrollPosY) {
    // Zoom In/Out
    this->fov -= (float)scrollPosY;
    if(this->fov > this->maxFOV) {
        this->fov = this->maxFOV;
    } if(this->fov < this->minFOV) {
        this->fov = this->minFOV;
    }
}

// Getters
glm::vec3 Camera::getPos(void) {return this->position;}
glm::vec3 Camera::getFront(void) {return this->front;}
glm::mat4 Camera::getViewMatrix(void) {return glm::lookAt(this->position, this->position + this->front, this->up);}
float Camera::getFOV(void) {return this->fov;}
