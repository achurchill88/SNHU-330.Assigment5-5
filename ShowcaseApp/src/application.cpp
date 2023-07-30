#include <application.h>  // Include necessary headers
#include <iostream>
#include <types.h>
#include <vector>
#include <glm/gtc/matrix_transform.hpp>
#include <cylinder.h>
#include "conicalfrustum.h"
#include <stb_image.h>

Application::Application(std::string WindowTitle, int width, int height)
        : _applicationName{std::move(WindowTitle)}, _width{width}, _height{height},
          _camera{width, height, {0.5f, 0.f, 3.f}, true},
          _cameraLookSpeed {0.02f, 0.02f}
{
    // Constructor that initializes the member variables
}

void Application::Run() {
    // Open window
    if (!openWindow()) {
        return;  // Return early if window opening fails
    }

    setupInputs();
    _running = true;

    // Setup the scene
    setupScene();

    // Run application
    while (_running) {
        float currentTime = glfwGetTime();
        if (_lastFrameTime == -1.f) {
            _lastFrameTime = currentTime;
        }
        auto deltaTime = currentTime - _lastFrameTime;
        _lastFrameTime = currentTime;

        if (glfwWindowShouldClose(_window)) {
            _running = false;
            continue;  // Exit the loop if the window should close
        }

        // Update
        update(deltaTime);

        // Draw
        draw();
    }

    glfwTerminate();  // Cleanup and terminate GLFW
}

bool Application::openWindow() {
    // Initialize and configure GLFW
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    // Create a GLFW window
    _window = glfwCreateWindow(800, 600, "Andy Churchill", nullptr, nullptr);

    if (!_window) {
        std::cout << "Failed to create GLFW window!" << std::endl;
        glfwTerminate();
        return false;  // Return false if window creation fails
    }

    glfwMakeContextCurrent(_window);
    glfwSetWindowUserPointer(_window, (void *) this);

    glfwSetFramebufferSizeCallback(_window, [](GLFWwindow *window, int width, int height) {
        // Resized window callback function
        glViewport(0, 0, width, height);

        auto app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
        app->_width = width;
        app->_height = height;

        app->_camera.SetSize(width, height);
    });

    if (!gladLoadGLLoader((GLADloadproc) glfwGetProcAddress)) {
        std::cerr << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
        return false;  // Return false if GLAD initialization fails
    }
    glEnable(GL_DEPTH_TEST);
    return true;  // Return true if window opening and initialization succeed
}

void Application::setupInputs() {
    glfwSetKeyCallback(_window, [](GLFWwindow *window, int key, int scancode, int action, int mods) {
        // std::cout << "Key Button (" << key << ", " << scancode << ", " << action << ", " << mods<< ")" << std::endl;
        auto *app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));

        switch (key) {
            case GLFW_KEY_ESCAPE: {
                if (action == GLFW_PRESS) {
                    app->_running = false;
                }
                break;
            }
            case GLFW_KEY_F11:{
                if (action == GLFW_PRESS) {
                    app->_camera.SetIsPerspective(!app->_camera.IsPerspective());
                }
            }
            default: {}
        }
    });

    glfwSetCursorPosCallback(_window, [](GLFWwindow* window, double xpos, double ypos) {
        auto *app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
        app->mousePositionCallback(xpos, ypos);
    });

    glfwSetScrollCallback(_window, [](GLFWwindow* window, double xOffset, double yOffset){
        auto *app = reinterpret_cast<Application *>(glfwGetWindowUserPointer(window));
        app->_camera.IncrementSpeed(yOffset * .2);
    });
}

// Function to set up the scene
void Application::setupScene() {
    // Create a pyramid mesh and add it to the "_meshes" container.
    auto& pyramidA = _meshes.emplace_back(Shapes::pyramidVertices, Shapes::pyramidElements);

    // Translate the position of "pyramidA" by (-1.f, 0.f, 0.f) using glm::translate.
    // This moves the pyramid to the left along the X-axis.
    pyramidA.Transform = glm::translate(pyramidA.Transform, glm::vec3(-1.f, 0.f, 0.f));

    // Set up the path to the "shaders" directory in the "assets" folder.
    Path shaderPath = std::filesystem::current_path() / "assets" / "shaders";

    // Create a Shader object using the vertex and fragment shader files located in the "shaders" directory.
    _shader = Shader(shaderPath / "basic_shader.vert", shaderPath / "basic_shader.frag");

    // Set up the path to the "textures" directory in the "assets" folder.
    Path texturePath = std::filesystem::current_path() / "assets" / "textures";

    // Create a full path to the "wall.jpg" texture file within the "textures" directory.
    auto containerPath = (texturePath / "wall.jpg").string();

    // Variables to store the width, height, and number of color channels of the loaded texture.
    int width, height, numChannels;

    // Use the STB image library to load the texture data from the "wall.jpg" file.
    // The function stbi_load() loads the image data and provides information about the texture.
    unsigned char* data = stbi_load(containerPath.c_str(), &width, &height, &numChannels, STBI_rgb_alpha);

    // Generate a new OpenGL texture and bind it to the GL_TEXTURE_2D target.
    // The texture's ID will be stored in the "_containerTexture" variable for future reference.
    glGenTextures(1, &_containerTexture);
    glBindTexture(GL_TEXTURE_2D, _containerTexture);

    // Check if the texture data was loaded successfully.
    if (data) {
        // Allocate storage for the texture using glTexStorage2D, specifying the texture's format, width, and height.
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);

        // Upload the texture data to the GPU using glTexSubImage2D.
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);

        // Generate mipmaps for the texture to enable smooth rendering at different distances.
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        // If loading fails, print an error message to the standard error stream.
        std::cerr << "Failed to load texture at path: " << containerPath << std::endl;
    }

    // Free the memory allocated for the loaded texture data, as it is no longer needed.
    stbi_image_free(data);
}


bool Application::update(float deltaTime) {
    glfwPollEvents();

    handleInput(deltaTime);

    return false;
}

bool Application::draw() {
    glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glm::mat4 view = _camera.GetViewMatrix();
    glm::mat4 projection = _camera.GetProjectionMatrix();

    _shader.Bind();
    _shader.SetMat4("projection", projection);
    _shader.SetMat4("view", view);

    // Binds the OpenGL texture with ID _containerTexture to GL_TEXTURE_2D target for use in texture sampling and rendering.
    glBindTexture(GL_TEXTURE_2D, _containerTexture);

//    bool _wireFrame{false};
    for (auto &mesh: _meshes) {
//        glPolygonMode(GL_FRONT_AND_BACK, _wireFrame ? GL_LINE : GL_FILL);
        mesh.Transform = glm::rotate(mesh.Transform, glm::radians(0.1f),
                                     glm::vec3(0, 1, 0)); // allows the object to rotate
        _shader.SetMat4("model", mesh.Transform);
        mesh.Draw();
//        _wireFrame = !_wireFrame;
    }

    glfwSwapBuffers(_window);
    return false;
}

void Application::handleInput(float deltaTime) {

    auto moveAmount = _camera.GetSpeed() * deltaTime * 4;

    if(glfwGetKey(_window, GLFW_KEY_W)){
        _camera.MoveCamera(Camera::MoveDirection::Forward, moveAmount);
    }
    if(glfwGetKey(_window, GLFW_KEY_A)){
        _camera.MoveCamera(Camera::MoveDirection::Left, moveAmount);
    }
    if(glfwGetKey(_window, GLFW_KEY_S)){
        _camera.MoveCamera(Camera::MoveDirection::Backward, moveAmount);
    }
    if(glfwGetKey(_window, GLFW_KEY_D)){
        _camera.MoveCamera(Camera::MoveDirection::Right, moveAmount);
    }
    if(glfwGetKey(_window, GLFW_KEY_Q)){
        _camera.MoveCamera(Camera::MoveDirection::Up, moveAmount);
    }
    if(glfwGetKey(_window, GLFW_KEY_E)){
        _camera.MoveCamera(Camera::MoveDirection::Down, moveAmount);
    }

    double xpos, ypos;
    glfwGetCursorPos(_window, &xpos, &ypos);
    mousePositionCallback(xpos, ypos);
}

void Application::mousePositionCallback(double xpos, double ypos) {
    if (!_firstMouse) {
        _lastMousePosition.x = static_cast<float>(xpos);
        _lastMousePosition.y = static_cast<float>(ypos);
        _firstMouse = true;
    }

    glm::vec2 moveAmount {
        xpos - _lastMousePosition.x,
        _lastMousePosition.y - ypos
    };
    _lastMousePosition.x = static_cast<float>(xpos);
    _lastMousePosition.y = static_cast<float>(ypos);

    _camera.RotateBy(moveAmount.x * _camera.GetSpeed(), moveAmount.y * _camera.GetSpeed());
}

