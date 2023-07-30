//
// Created by churc on 7/29/2023.
//

#include <texture.h>
#include <stb_image.h>
#include <iostream>

Texture::Texture(const std::filesystem::path &path) {
    stbi_set_flip_vertically_on_load(false);

    // Set up the path to the "textures" directory in the "assets" folder.

    auto texturePath = path.string();
    int width, height, numChannels;
    unsigned char* data = stbi_load(texturePath.c_str(), &width, &height, &numChannels, STBI_rgb_alpha);
    glGenTextures(1, &_textureHandle);
    glBindTexture(GL_TEXTURE_2D, _textureHandle);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
//    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);


    if (data) {
        glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, width, height);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, width, height, GL_RGBA, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
    } else {
        std::cerr << "Failed to load texture at path: " << texturePath << std::endl;
    }
    stbi_image_free(data);

}

void Texture::Bind() {
    glBindTexture(GL_TEXTURE_2D, _textureHandle);
}
