//
// Created by churc on 7/29/2023.
//

#pragma once
#include <filesystem>
#include <glad/glad.h>

class Texture {
public:
    Texture(const std::filesystem::path& path);
    void Bind();
private:
    GLuint _textureHandle;
};