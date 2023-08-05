//
// Created by churc on 7/13/2023.
//

#pragma once
#include <filesystem>
#include <string>
#include <glad/glad.h>
#include <glm/glm.hpp>

using Path = std::filesystem::path;

class Shader {
public:
    Shader() = default;

    Shader(const std::string& vertexSource, const std::string& fragmentSource);  // Constructor with shader source code
    Shader(const Path& vertexPath, const Path& fragmentPath);  // Constructor with shader file paths

    void Bind();  // Function to bind the shader program

    void SetMat4(const std::string& uniformName, const glm::mat4& mat4);  // Function to set a 4x4 matrix uniform
    void SetInt(const std::string& uniformName, int value);
    void SetVec3(const std::string& uniformName, const glm::vec3& vec3);
    void SetFloat(const std::string& uniformName, float value);

private:
    void load(const std::string& vertexSource, const std::string& fragmentSource);  // Function to load and compile the shader program
    GLint getUniformLocation(const std::string& uniformName);  // Function to get the location of a uniform variable

private:
    GLuint _shaderProgram;  // ID of the shader program
};
