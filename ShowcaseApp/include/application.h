#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <string>
#include <mesh.h>
#include <shader.h>
#include <types.h>
#include <camera.h>
#include "texture.h"

class Application {
public:
    Application();  // Default constructor
    Application(std::string WindowTitle, int width, int height);  // Constructor with parameters


    void Run();  // Function to run the application

private:
    bool openWindow();  // Function to open the application window
    void setupInputs();

    void setupScene();  // Function to set up the scene
    bool update(float deltaTime);  // Function to update the application state
    bool draw();  // Function to draw the scene

    void handleInput(float deltaTime);

    void mousePositionCallback(double xpos, double ypos);

private:
    std::string _applicationName;  // Name of the application window
    int _width{};  // Width of the application window
    int _height{};  // Height of the application window
    GLFWwindow *_window{nullptr};  // Pointer to the GLFW window

    Camera _camera;
    std::vector<Mesh> _meshes;  // Vector to store meshes in the scene
    std::vector<Texture> _textures;
    Shader _shader;  // Shader object for rendering
    bool _running{false};  // Flag indicating whether the application is running

    bool _firstMouse { false };  // Flag to track the first mouse movement
    glm::vec2 _lastMousePosition {-1, -1};  // Last recorded mouse position
    glm::vec2 _cameraLookSpeed {};  // Speed at which the camera looks around

    float _lastFrameTime { 1.f };  // Time of the last frame for deltaTime calculation
};
