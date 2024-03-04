// Custom Engine Header
#define SOKOL_IMPL
#define SOKOL_GLCORE33
#define STB_IMAGE_IMPLEMENTATION
#include"Engine/Engine.hpp"

// Include Shaders
#include"includeShaders.h"

// Function Forward Declerations
static void ErrorCallback(int errorCode, const char* errorDesc);
static void FramebufferSizeCallback(GLFWwindow*, int newWinWidth, int newWinHeight);
static void MouseCallback(GLFWwindow*, double mousePosX, double mousePosY);
static void ScrollCallback(GLFWwindow*, double, double scrollPosY);
static void processKeyboardInput(GLFWwindow* win);

// Variables
// General Config
const bool vsyncEnabled {true};

// Main Window Config
const std::string windowTitle {"LearningSokol"};
int windowWidth {800}, windowHeight {600};
const bool windowFullscreen {true};
const float windowBgColor[4] {0.2f, 0.3f, 0.3f, 1.0f};

// Delta Time
float lastTime, currentTime, deltaTime;

// Camera
sgl::Camera camera(glm::vec3(0.0f, 0.0f, 3.0f), glm::vec3(0.0f, 0.0f, -1.0f), 1.5f, 0.3f, 70.0f, 0.1f, 120.0f);

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
#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GLFW_TRUE);
#endif

    // Main Window
    GLFWwindow* win {nullptr};
    if(windowFullscreen) {
        // Get Current Physical Monitor Dimensions
        windowWidth = glfwGetVideoMode(glfwGetPrimaryMonitor())->width;
        windowHeight = glfwGetVideoMode(glfwGetPrimaryMonitor())->height;

        // Create Main Window - Fullscreen and Borderless
        win = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), glfwGetPrimaryMonitor(), nullptr);
    } else {
        // Create Main Window - Windowed and Bordered
        win = glfwCreateWindow(windowWidth, windowHeight, windowTitle.c_str(), nullptr, nullptr);
    }

    // Check if Main Window Was Created Successfully
    if(win == nullptr) {
        // Terminate GLFW and Return Failure Exit Code
        glfwTerminate();
        return EXIT_FAILURE;
    }

    // Set Current OpenGL Render Context
    glfwMakeContextCurrent(win);

    // Set GLFW Input Mode
    glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Set GLFW Callbacks
    glfwSetFramebufferSizeCallback(win, FramebufferSizeCallback);
    glfwSetCursorPosCallback(win, MouseCallback);
    glfwSetScrollCallback(win, ScrollCallback);

    // Initialize Sokol
    sg_desc sokolSetupDesc {};
    sokolSetupDesc.logger.func = slog_func;
    sg_setup(&sokolSetupDesc);

    // Triangle Data
    const float triangleVertices[] {
        -0.4f, -0.5f, 0.0f, 0.0f, 0.0f,
        0.4f, -0.5f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.5f, 0.0f, 0.5f, 1.0f,
    };
    const uint16_t triangleIndices[] {
        0, 1, 2,
    };

    // Vertex Buffer Objects
    // Triangle VBO
    sg_buffer_desc triangleVBODesc {};
    triangleVBODesc.type = SG_BUFFERTYPE_VERTEXBUFFER;
    triangleVBODesc.data = SG_RANGE_REF(triangleVertices);
    sg_buffer triangleVBO {sg_make_buffer(&triangleVBODesc)};

    // Index/Element Buffer Objects
    // Triangle IBO
    sg_buffer_desc triangleIBODesc {};
    triangleIBODesc.type = SG_BUFFERTYPE_INDEXBUFFER;
    triangleIBODesc.data = SG_RANGE_REF(triangleIndices);
    sg_buffer triangleIBO {sg_make_buffer(&triangleIBODesc)};

    // Shaders
    sg_shader testShader {sg_make_shader(testShader_shader_desc(sg_query_backend()))};

    // Textures
    sg_sampler_desc testTexSmpDesc {};
    testTexSmpDesc.min_filter = SG_FILTER_NEAREST;
    testTexSmpDesc.mag_filter = SG_FILTER_NEAREST;
    sg_sampler testTexSmp {sg_make_sampler(testTexSmpDesc)};

    sg_image_desc testTexImgDesc {};
    testTexImgDesc.type = SG_IMAGETYPE_2D;
    testTexImgDesc.pixel_format = SG_PIXELFORMAT_RGBA8;
    testTexImgDesc.usage = SG_USAGE_IMMUTABLE;
    sgl::Texture testTex(testTexImgDesc, "../assets/textures/wall.jpg");

    // Bindings
    // Test Object Bindings
    sg_bindings testObjBindings {};
    testObjBindings.vertex_buffers[0] = triangleVBO;
    testObjBindings.index_buffer = triangleIBO;
    testObjBindings.fs.images[SLOT_testTexImg] = testTex.getImage();
    testObjBindings.fs.samplers[SLOT_testTexSmp] = testTexSmp;

    // Pipeline Objects
    // Test Object Pipeline
    sg_pipeline_desc testObjPipelineDesc {};
    testObjPipelineDesc.shader = testShader;
    testObjPipelineDesc.layout.attrs[ATTR_vs_vertexPosition].format = SG_VERTEXFORMAT_FLOAT3;
    testObjPipelineDesc.layout.attrs[ATTR_vs_textureCoords].format = SG_VERTEXFORMAT_FLOAT2;
    testObjPipelineDesc.index_type = SG_INDEXTYPE_UINT16;
    testObjPipelineDesc.depth.compare = SG_COMPAREFUNC_LESS;
    testObjPipelineDesc.depth.write_enabled = true;
    testObjPipelineDesc.primitive_type = SG_PRIMITIVETYPE_TRIANGLES;
    sg_pipeline testObjPipeline {sg_make_pipeline(&testObjPipelineDesc)};

    // Render Pass Action
    sg_pass_action renderPassAction {};
    renderPassAction.colors[0].load_action = SG_LOADACTION_CLEAR;
    renderPassAction.colors[0].clear_value = {windowBgColor[0], windowBgColor[1], windowBgColor[2], windowBgColor[3]};

    // Model and Projection Matrices
    glm::mat4 modelMatrix, projectionMatrix;

    // Main Loop
    while(!glfwWindowShouldClose(win)) {
        // Swap Buffers
        glfwSwapBuffers(win);

        // Projection Matrix
        projectionMatrix = glm::mat4(1.0f);
        projectionMatrix = glm::perspective(glm::radians(camera.getFOV()), static_cast<float>(windowWidth) / static_cast<float>(windowHeight), 0.1f, 100.0f);

        // Begin Render Pass
        sg_begin_default_pass(renderPassAction, windowWidth, windowHeight);

        // Render Test Object
        sg_apply_pipeline(testObjPipeline);
        sg_apply_bindings(&testObjBindings);
        modelMatrix = glm::mat4(1.0f);
        modelMatrix = glm::rotate(modelMatrix, static_cast<float>(glfwGetTime()) * glm::radians(40.0f), glm::vec3(0.0f, 1.0f, 0.0f));
        testShaderVSUniforms.pvm = projectionMatrix * camera.getViewMat() * modelMatrix;
        sg_apply_uniforms(SG_SHADERSTAGE_VS, SLOT_testVSUniforms, SG_RANGE_REF(testShaderVSUniforms));
        sg_draw(0, 3, 1);

        // End Render Pass and Submit Frame
        sg_end_pass();
        sg_commit();

        // Calculate Delta Time
        currentTime = static_cast<float>(glfwGetTime());
        deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        // Process User Keyboard Input and Poll Events
        processKeyboardInput(win);
        glfwPollEvents();
    }

    // Terminate Program
    // Shutdown Sokol
    sg_shutdown();

    // Destroy Main Window and Terminate GLFW
    glfwDestroyWindow(win);
    glfwSetWindowUserPointer(win, nullptr);
    glfwTerminate();

    // Return Successful Exit Code
    return EXIT_SUCCESS;
}

// Process User Keyboard Input
static void processKeyboardInput(GLFWwindow* win) {
    // Escape Key Pressed - Terminate Program
    if(glfwGetKey(win, GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        glfwSetWindowShouldClose(win, GLFW_TRUE);
    }

    // Camera Process Keyboard Input - Move Around
    camera.processKeyboard(win, deltaTime);
}

// GLFW Callbacks
static void ErrorCallback(int errorCode, const char* errorDesc) {
    // Display GLFW Error Code and Description
    std::cerr << "GLFW Error " << errorCode << ": " << errorDesc << std::endl;
}
static void FramebufferSizeCallback(GLFWwindow*, int newWinWidth, int newWinHeight) {
    // Resize Render Viewport and Save The New Window Dimensions
    sg_apply_viewport(0, 0, newWinWidth, newWinHeight, true);
    windowWidth = newWinWidth;
    windowHeight = newWinHeight;
}
static void MouseCallback(GLFWwindow*, double mousePosX, double mousePosY) {
    // Camera Process Mouse Movement - Look Around
    camera.processMouse(mousePosX, mousePosY);
}
static void ScrollCallback(GLFWwindow*, double, double scrollPosY) {
    // Camera Process Scroll - Zoom In/Out
    camera.processScroll(scrollPosY);
}
