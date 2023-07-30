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

    Path texturePath = std::filesystem::current_path() / "assets" / "textures";
    _textures.emplace_back(texturePath / "bottle.jpg");
    _textures.emplace_back(texturePath / "rootbeerLabel.jpg");

    // Define the scale factor
    float scaleFactor = 0.90f;

    // Create a cylinder
    float bottleRadius = 0.25f * scaleFactor; // Reduced radius
    float bottleHeight = 2.25f * scaleFactor; // Reduced height
    int bottleSectors = 64;
    Cylinder cylinder(bottleRadius, bottleHeight, bottleSectors);
    auto cylinderMesh = cylinder.GetMesh();
    cylinderMesh.Transform = glm::scale(glm::mat4(1.0f), glm::vec3(scaleFactor, scaleFactor, scaleFactor)) *
                             glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, bottleHeight / 2.0f, 0.0f)) *
                             cylinderMesh.Transform;
    cylinderMesh.SetTextures({_textures[1], _textures[0]});

    _meshes.emplace_back(cylinderMesh);

    // Create a conical frustum for the middle part of the bottle
    float middleBottleTopRadius = 0.15f * scaleFactor; // Reduced top radius
    float middleBottleBottomRadius = 0.25f * scaleFactor; // Reduced bottom radius
    float middleBottleHeight = 0.5f * scaleFactor; // Reduced height
    int middleBottleSectors = 64;
    ConicalFrustum middleConicalFrustum(middleBottleTopRadius, middleBottleBottomRadius, middleBottleHeight,
                                        middleBottleSectors);
    auto middleConicalFrustumMesh = middleConicalFrustum.GetMesh();
    middleConicalFrustumMesh.Transform = glm::scale(glm::mat4(1.0f), glm::vec3(scaleFactor, scaleFactor, scaleFactor)) *
                                         glm::translate(glm::mat4(1.0f),
                                                        glm::vec3(0.0f, bottleHeight + middleBottleHeight / 2.0f,
                                                                  0.0f)) * middleConicalFrustumMesh.Transform;
    middleConicalFrustumMesh.SetTextures({_textures[0]});
    _meshes.emplace_back(middleConicalFrustumMesh);

    // Create a conical frustum for the top part of the bottle
    float topBottleTopRadius = 0.1f * scaleFactor; // Reduced top radius
    float topBottleBottomRadius = 0.15f * scaleFactor; // Reduced bottom radius
    float topBottleHeight = 1.5f * scaleFactor; // Reduced height
    int topBottleSectors = 64;
    ConicalFrustum topConicalFrustum(topBottleTopRadius, topBottleBottomRadius, topBottleHeight, topBottleSectors);
    auto topConicalFrustumMesh = topConicalFrustum.GetMesh();
    topConicalFrustumMesh.Transform = glm::scale(glm::mat4(1.0f), glm::vec3(scaleFactor, scaleFactor, scaleFactor)) *
                                      glm::translate(glm::mat4(1.0f), glm::vec3(0.0f,
                                                                                bottleHeight + middleBottleHeight +
                                                                                topBottleHeight / 2.0f, 0.0f)) *
                                      topConicalFrustumMesh.Transform;
    topConicalFrustumMesh.SetTextures({_textures[0]});
    _meshes.emplace_back(topConicalFrustumMesh);

    // Plane
    _meshes.emplace_back(Shapes::tableTopVertices, Shapes::tableTopElements);


    // Set up the path to the "shaders" directory in the "assets" folder.
    Path shaderPath = std::filesystem::current_path() / "assets" / "shaders";

    // Create a Shader object using the vertex and fragment shader files located in the "shaders" directory.
    _shader = Shader(shaderPath / "basic_shader.vert", shaderPath / "basic_shader.frag");

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

    // Loop through the meshes and draw them with their respective textures
    for (size_t i = 0; i < _meshes.size(); i++) {
        Mesh& mesh = _meshes[i];
        std::vector<Texture>& textures = mesh.GetTextures();
        
        for (size_t j = 0; j < textures.size(); j++) {
            glActiveTexture(GL_TEXTURE0 + j);
            textures[j].Bind();
            _shader.SetInt("tex" + std::to_string(j), j); // Set the uniform value dynamically
        }

        _shader.SetMat4("model", mesh.Transform);
        mesh.Draw();
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