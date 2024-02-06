// Standard Headers
#include<iostream>
#include<cstdlib>

// Sokol
#define SOKOL_GFX_IMPL
#define SOKOL_LOG_IMPL
#define SOKOL_GLCORE33
#include<sokol_gfx.h>
#include<sokol_log.h>

// GLFW
#define GLFW_INCLUDE_NONE
#include<GLFW/glfw3.h>

// OpenGL Mathematics - GLM
#include<glm/glm.hpp>
#include<glm/gtc/matrix_transform.hpp>

// Custom Engine Headers
#include"Engine/Camera/Camera.hpp"

// Shaders
#include"shaders/includeShaders.hpp"

// Function Forward Declerations
void ErrorCallback(int errorCode, const char* errorDesc);
void MouseCallback(GLFWwindow*, double mousePosX, double mousePosY);
void ScrollCallback(GLFWwindow*, double scrollPosX, double scrollPosY);
void processKeyboardInput(GLFWwindow* win);

// Variables
// Game Config
const bool vsyncEnabled {true}, msaaEnabled {true};
const unsigned int msaaSamples {4};

// Main Window Config
const std::string windowTitle {"LearningSokol"};
int windowWidth {800}, windowHeight {600};
float windowAspectRatio;
const bool windowFullscreen {true};
const float windowBgColor[4] = {0.2f, 0.3f, 0.3f, 1.0f};

// Delta Time
float lastTime, currentTime, deltaTime;

// Perspective Camera
Camera perspectiveCamera(glm::vec3(0.0f, 0.0f, 0.0f), 2.5f, 0.3f, 70.0f, 0.1f, 120.0f);
const float perspectiveCameraNear {0.1f}, perspectiveCameraFar{100.0f};

// Shader Uniform Blocks
// Test Shader
typedef struct {
    // Product of Projection, View and Model Matrices
    glm::mat4 pvm;
} testShaderUniformBlock_0;
testShaderUniformBlock_0 testShaderUniformBlock_0_Uniforms;

// Main
int main(void) {
    // Set GLFW Error Callback
    glfwSetErrorCallback(ErrorCallback);

    // Initialize GLFW
    if(!glfwInit()) return EXIT_FAILURE;

    // Set Window Hints
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
    msaaEnabled ? glfwWindowHint(GLFW_SAMPLES, msaaSamples) : glfwWindowHint(GLFW_SAMPLES, 0);

    // Main Window
    GLFWwindow* win = nullptr;
    if(windowFullscreen) {
        // Get Physical Monitor Dimensions
        windowWidth = glfwGetVideoMode(glfwGetPrimaryMonitor())->width;
        windowHeight = glfwGetVideoMode(glfwGetPrimaryMonitor())->height;

        // Calculate Window Aspect Ratio
        windowAspectRatio = (float)windowWidth / (float)windowHeight;

        // Create Main Window - Fullscreen and Borderless
        win = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), glfwGetPrimaryMonitor(), nullptr);
    } else {
        // Calculate Window Aspect Ratio
        windowAspectRatio = (float)windowWidth / (float)windowHeight;

        // Create Main Window - Windowed and Bordered
        win = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), nullptr, nullptr);
    }

    // Check if Main Window Was Created Successfully
    if(win == nullptr) {
        // Terminate GLFW and Return Failure Exit Code
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Set Current OpenGL Rendering Context
    glfwMakeContextCurrent(win);

    // Set GLFW Input Mode
    glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set GLFW Callbacks
    glfwSetCursorPosCallback(win, MouseCallback);
    glfwSetScrollCallback(win, ScrollCallback);

    // Initialize Sokol
    sg_desc sokolSetupDesc {};
    sokolSetupDesc.logger.func = slog_func;
    sg_setup(sokolSetupDesc);

    // Render Pass Action
    sg_pass_action renderPassAction {};
    renderPassAction.colors[0].load_action = SG_LOADACTION_CLEAR;
    renderPassAction.colors[0].clear_value = {windowBgColor[0], windowBgColor[1], windowBgColor[2], windowBgColor[3]};

    // Cube Data
    const float cubeVertices[] = {
        -1.0, -1.0, -1.0,
         1.0, -1.0, -1.0,
         1.0,  1.0, -1.0,
        -1.0,  1.0, -1.0,

        -1.0, -1.0,  1.0,
         1.0, -1.0,  1.0,
         1.0,  1.0,  1.0,
        -1.0,  1.0,  1.0,

        -1.0, -1.0, -1.0,
        -1.0,  1.0, -1.0,
        -1.0,  1.0,  1.0,
        -1.0, -1.0,  1.0,

        1.0, -1.0, -1.0,
        1.0,  1.0, -1.0,
        1.0,  1.0,  1.0,
        1.0, -1.0,  1.0,

        -1.0, -1.0, -1.0,
        -1.0, -1.0,  1.0,
         1.0, -1.0,  1.0,
         1.0, -1.0, -1.0,

        -1.0,  1.0, -1.0,
        -1.0,  1.0,  1.0,
         1.0,  1.0,  1.0,
         1.0,  1.0, -1.0,
    };
    const uint16_t cubeIndices[] = {
        0, 1, 2,  0, 2, 3,
        6, 5, 4,  7, 6, 4,
        8, 9, 10,  8, 10, 11,
        14, 13, 12,  15, 14, 12,
        16, 17, 18,  16, 18, 19,
        22, 21, 20,  23, 22, 20,
    };

    // Vertex Buffer Objects
    // Cube VBO
    sg_buffer_desc cubeVBODesc {};
    cubeVBODesc.type = SG_BUFFERTYPE_VERTEXBUFFER;
    cubeVBODesc.data = SG_RANGE(cubeVertices);
    sg_buffer cubeVBO {sg_make_buffer(cubeVBODesc)};

    // Index/Element Buffer Objects
    // Cube IBO
    sg_buffer_desc cubeIBODesc {};
    cubeIBODesc.type = SG_BUFFERTYPE_INDEXBUFFER;
    cubeIBODesc.data = SG_RANGE(cubeIndices);
    sg_buffer cubeIBO {sg_make_buffer(cubeIBODesc)};

    // Shaders
    // Test Shader
    sg_shader testShader {sg_make_shader(testShaders_shader_desc(sg_query_backend()))};

    // Resource Bindings
    // Test Object Bindings
    sg_bindings testObjBindings {};
    testObjBindings.vertex_buffers[0] = cubeVBO;
    testObjBindings.index_buffer = cubeIBO;

    // Pipelines
    // Test Object Pipeline
    sg_pipeline_desc testObjPipelineDesc {};
    testObjPipelineDesc.shader = testShader;
    testObjPipelineDesc.layout.attrs[0].format = SG_VERTEXFORMAT_FLOAT3;
    testObjPipelineDesc.depth.compare = SG_COMPAREFUNC_LESS;
    testObjPipelineDesc.depth.write_enabled = true;
    testObjPipelineDesc.index_type = SG_INDEXTYPE_UINT16;
    sg_pipeline testObjPipeline {sg_make_pipeline(testObjPipelineDesc)};

    // Model, View, Projection Matrices and Their Product
    glm::mat4 modelMatrix, viewMatrix, projectionMatrix, pvm;

    // Main Loop
    while(!glfwWindowShouldClose(win)) {
        // Swap Buffers
        glfwSwapBuffers(win);

        // View Matrix
        viewMatrix = glm::mat4(1.0f);
        viewMatrix = perspectiveCamera.getViewMatrix();

        // Projection Matrix
        projectionMatrix = glm::mat4(1.0f);
        projectionMatrix = glm::perspective(glm::radians(perspectiveCamera.getFOV()), windowAspectRatio, perspectiveCameraNear, perspectiveCameraFar);

        // Begin Render Pass
        sg_begin_default_pass(renderPassAction, windowWidth, windowHeight);

        // Prepare to Render Test Object
        // Apply Pipeline and Resource Binds
        sg_apply_pipeline(testObjPipeline);
        sg_apply_bindings(testObjBindings);

        // Test Object's Model Matrix
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::rotate(modelMatrix, (float)glfwGetTime() * glm::radians(20.0f), glm::vec3(0.0f, 1.0f, 0.0f));

        // Send Product of Projection, View and Model Matrices to Test Shader
        testShaderUniformBlock_0_Uniforms.pvm = projectionMatrix * viewMatrix * modelMatrix;
        sg_apply_uniforms(SG_SHADERSTAGE_VS, 0, SG_RANGE(testShaderUniformBlock_0_Uniforms));

        // Render Test Object
        sg_draw(0, 36, 1);

        // End Render Pass and Submit Frame
        sg_end_pass();
        sg_commit();

        // Calculate Delta Time
        currentTime = glfwGetTime();
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Process Keyboard Input
        processKeyboardInput(win);

        // Poll Events
        glfwPollEvents();
    }

    // Terminate Program
    // Delete Vertex Buffer Objects
    sg_destroy_buffer(cubeVBO);

    // Delete Index/Element Buffer Objects
    sg_destroy_buffer(cubeIBO);

    // Delete Shaders
    sg_destroy_shader(testShader);

    // Delete Pipelines
    sg_destroy_pipeline(testObjPipeline);

    // Terminate Sokol
    sg_shutdown();

    // Destroy Main Window and Terminate GLFW
    glfwDestroyWindow(win);
    win = nullptr;
    glfwTerminate();

    // Return Successful Exit Code
    return EXIT_SUCCESS;
}

// GLFW Error Callback
void ErrorCallback(int errorCode, const char* errorDesc) {
    // Display GLFW Error Code and Description
    std::cerr << "GLFW Error " << errorCode << ": " << errorDesc << "\n";
}

// GLFW Mouse Callback
void MouseCallback(GLFWwindow*, double mousePosX, double mousePosY) {
    // Camera Handle Mouse Movement - Look Around
    perspectiveCamera.handleMouseInput(mousePosX, mousePosY);
}

// GLFW Scroll Wheel Callback
void ScrollCallback(GLFWwindow*, double scrollPosX, double scrollPosY) {
    // Camera Handle Scroll Wheel - Zoom In/Out
    perspectiveCamera.handleScrollInput(scrollPosY);
}

// Process Keyboard Input
void processKeyboardInput(GLFWwindow* win) {
    // Escape Key Pressed - Terminate Program
    if(glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(win, GLFW_TRUE);
    }

    // Camera Handle Keyboard Input - Move Around
    perspectiveCamera.handleKeyboardInput(win, deltaTime);
}
