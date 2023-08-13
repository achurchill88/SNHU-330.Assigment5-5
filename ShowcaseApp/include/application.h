// application.h
#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>
#include <vector>
#include <filesystem>
#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include "camera.h"
#include "shader.h"
#include "mesh.h"
#include "texture.h"
#include "types.h"

class Application {
public:
    Application(std::string WindowTitle, int width, int height);

    void Run();

private:
    std::string _applicationName;
    int _width;
    int _height;
    GLFWwindow* _window;
    bool _running;
    float _lastFrameTime = -1.f;

    Camera _camera;
    glm::vec2 _lastMousePosition;
    bool _firstMouse = false;
    glm::vec2 _cameraLookSpeed;

    std::vector<Mesh> _meshes;
    std::vector<Texture> _textures;
    Shader _shader;

    bool openWindow();
    void setupInputs();
    void setupScene();
    bool update(float deltaTime);
    bool draw();
    void handleInput(float deltaTime);
    void mousePositionCallback(double xpos, double ypos);
};

#endif // APPLICATION_H
