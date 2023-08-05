#include <application.h>
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
          _cameraLookSpeed {0.02f, 0.02f},
          _lightDirection{glm::vec3(0.5f, 1.0f, 0.5f)}, // Default directional light direction
          _lightColor{glm::vec3(0.0f, 1.0f, 0.0f)}, // Default greenish light color
          _pointLightPosition{glm::vec3(1.0f, 2.0f, 2.0f)}, // Default point light position
          _pointLightColor{glm::vec3(1.0f, 1.0f, 1.0f)} // Default white point light color
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

    auto& pyramidA = _meshes.emplace_back(Shapes::pyramidVertices, Shapes::pyramidElements);
    pyramidA.Transform = glm::translate(pyramidA.Transform, glm::vec3(-1.f, 0.f, 0.f));

    // Set up the path to the "shaders" directory in the "assets" folder.
    Path shaderPath = std::filesystem::current_path() / "assets" / "shaders";

    // Create a Shader object using the vertex and fragment shader files located in the "shaders" directory.
    _shader = Shader(shaderPath / "basic_shader.vert", shaderPath / "basic_shader.frag");
    
    // Set up lighting information
    _lightDirection = glm::normalize(glm::vec3(0.5f, 1.0f, 0.5f)); // Directional light direction
    _lightColor = glm::vec3(0.0f, 1.0f, 0.0f); // Greenish light color
    _pointLightPosition = glm::vec3(1.0f, 2.0f, 2.0f); // Point light position
    _pointLightColor = glm::vec3(1.0f, 1.0f, 1.0f); // White point light color

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
    _shader.SetVec3("lightDirection", _lightDirection);
    _shader.SetVec3("lightColor", _lightColor);
    _shader.SetVec3("pointLightPosition", _pointLightPosition);
    _shader.SetVec3("pointLightColor", _pointLightColor);

    // Calculate point light attenuation
    float constant = 1.0f;
    float linear = 0.01f;
    float quadratic = 0.01f;
    _shader.SetFloat("pointLightConstant", constant);
    _shader.SetFloat("pointLightLinear", linear);
    _shader.SetFloat("pointLightQuadratic", quadratic);

    // Loop through the meshes and draw them with their respective textures
    for (size_t i = 0; i < _meshes.size(); i++) {
        Mesh& mesh = _meshes[i];
        std::vector<Texture>& textures = mesh.GetTextures();
        
        for (size_t j = 0; j < textures.size(); j++) {
            glActiveTexture(GL_TEXTURE0 + j);
            textures[j].Bind();
            _shader.SetInt("tex0", j); // Set the texture uniform to GL_TEXTURE0
        }

        // Calculate point light contribution for the mesh
        glm::vec3 toPointLight = _pointLightPosition - glm::vec3(mesh.Transform[3]);
        float distanceToPointLight = glm::length(toPointLight);
        float attenuation = 1.0f / (constant + linear * distanceToPointLight + quadratic * (distanceToPointLight * distanceToPointLight));
        _shader.SetFloat("pointLightAttenuation", attenuation);

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