#ifndef APPLICATION_H
#define APPLICATION_H

#include <string>
#include <vector>
#include <glm/glm.hpp>
#include "shader.h"
#include "camera.h"
#include "mesh.h"
#include "texture.h"
#include <GLFW/glfw3.h>

class Application {
public:
    Application(std::string WindowTitle, int width, int height);
    void Run();

private:
    bool openWindow();
    void setupInputs();
    void setupScene();
    bool update(float deltaTime);
    bool draw();
    void handleInput(float deltaTime);
    void mousePositionCallback(double xpos, double ypos);

private:
    std::string _applicationName;
    int _width;
    int _height;
    GLFWwindow* _window;
    Camera _camera;
    glm::vec2 _cameraLookSpeed;
    glm::vec3 _lightDirection;
    glm::vec3 _lightColor;
    glm::vec3 _pointLightPosition;
    glm::vec3 _pointLightColor;
    std::vector<Mesh> _meshes;
    std::vector<Texture> _textures;
    Shader _shader;
    bool _running = false;
    float _lastFrameTime = -1.0f;
    bool _firstMouse = false;
    glm::vec2 _lastMousePosition;
};

#endif // APPLICATION_H
