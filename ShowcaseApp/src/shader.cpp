//
// Created by churc on 7/13/2023.
//

#include <iostream>
#include <Shader.h>
#include <fstream>
#include <glm/gtc/type_ptr.hpp>

Shader::Shader(const std::string& vertexSource, const std::string& fragmentSource) {
    load(vertexSource, fragmentSource);
}

Shader::Shader(const Path& vertexPath, const Path& fragmentPath) {
    try {
        std::ifstream vShaderFile;
        std::ifstream fShaderFile;

        // Open vertex shader file
        vShaderFile.open(vertexPath);
        // Open fragment shader file
        fShaderFile.open(fragmentPath);

        std::stringstream vShaderStream, fShaderStream;
        // Read the vertex shader file into a string stream
        vShaderStream << vShaderFile.rdbuf();
        // Read the fragment shader file into a string stream
        fShaderStream << fShaderFile.rdbuf();

        vShaderFile.close();
        fShaderFile.close();

        // Load shaders from string streams
        load(vShaderStream.str(), fShaderStream.str());
    } catch (std::ifstream::failure& e) {
        std::cout << "ERROR::SHADER::FILE_NOT_SUCCESSFULLY_READ" << std::endl;
    }
}

void Shader::Bind() {
    glUseProgram(_shaderProgram);
}

void Shader::load(const std::string& vertexSource, const std::string& fragmentSource) {
    const char* vShaderCode = vertexSource.c_str();
    const char* fShaderCode = fragmentSource.c_str();

    // Create vertex shader object and compile shader source
    auto vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vShaderCode, nullptr);
    glCompileShader(vertexShader);

    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::VERTEX::COMPILATION_FAILED" << infoLog << std::endl;
    }

    // Create fragment shader object and compile shader source
    auto fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fShaderCode, nullptr);
    glCompileShader(fragmentShader);

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);

    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::COMPILATION_FAILED" << infoLog << std::endl;
    }

    // Create shader program, attach shaders, and link program
    _shaderProgram = glCreateProgram();
    glAttachShader(_shaderProgram, vertexShader);
    glAttachShader(_shaderProgram, fragmentShader);
    glLinkProgram(_shaderProgram);

    glGetProgramiv(_shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(_shaderProgram, 512, nullptr, infoLog);
        std::cerr << "ERROR::SHADER::FRAGMENT::LINK_FAIL" << infoLog << std::endl;
    }

    // Delete shader objects as they are linked to the shader program
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

GLint Shader::getUniformLocation(const std::string& uniformName) {
    return glGetUniformLocation(_shaderProgram, uniformName.c_str());
}

void Shader::SetMat4(const std::string& uniformName, const glm::mat4& mat4) {
    auto uniformLoc = getUniformLocation(uniformName);
    if (uniformLoc != -1) {
        glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, glm::value_ptr(mat4));
    }
}

void Shader::SetInt(const std::string &uniformName, int value) {
    auto uniformLoc = getUniformLocation(uniformName);
    if (uniformLoc != -1) {
        glUniform1i(uniformLoc, value);
    }
}
