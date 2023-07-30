#pragma once

#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>
#include "types.h"
#include "texture.h"

class Mesh {
public:
    Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);  // Constructor with vertices and indices

    void Draw();  // Function to draw the mesh
    glm::mat4 Transform{ 1.0f };  // Transformation matrix for the mesh
    float GetHeight() const { return _height; }  // Method to retrieve the mesh height

    std::vector<Vertex> GetVertices() const { return _vertices; }  // Function to get the vertices of the mesh
    std::vector<uint32_t> GetIndices() const { return _indices; }  // Function to get the indices of the mesh

    void SetColor(glm::vec3 color) {  // Function to set the color of the mesh
        for (auto& vertex : _vertices) {
            vertex.Color = color;
        }
    }
    void SetTextures(const std::vector<Texture>& textures) { _textures = textures; }
    std::vector<Texture>& GetTextures() { return _textures; }

private:
    uint32_t _elementCount{ 0 };  // Number of elements (indices)
    GLuint _vertexBufferObject{};  // Vertex buffer object
    GLuint _vertexArrayObject{};  // Vertex array object
    GLuint _elementBufferObject{};  // Element buffer object
    float _height{ 0.0f };  // Height of the mesh

    std::vector<Vertex> _vertices;  // Vector to store the vertices of the mesh
    std::vector<uint32_t> _indices;  // Vector to store the indices of the mesh
    std::vector<Texture> _textures; // Vector to store textures associated with the mesh
};
