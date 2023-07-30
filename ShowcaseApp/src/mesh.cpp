#include "mesh.h"
#include <iostream>

Mesh::Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices)
        : _vertices(vertices), _indices(indices)
{
    // Create vertex array object, vertex buffer object, and element buffer object
    glGenVertexArrays(1, &_vertexArrayObject);
    glGenBuffers(1, &_vertexBufferObject);
    glGenBuffers(1, &_elementBufferObject);

    // Bind the vertex array object
    glBindVertexArray(_vertexArrayObject);

    // Bind and fill the vertex buffer object with vertex data
    glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferObject);
    glBufferData(GL_ARRAY_BUFFER, _vertices.size() * sizeof(Vertex), _vertices.data(), GL_STATIC_DRAW);

    // Bind and fill the element buffer object with index data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _elementBufferObject);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, _indices.size() * sizeof(uint32_t), _indices.data(), GL_STATIC_DRAW);

    // Calculate the height of the mesh
    float minY = FLT_MAX;
    float maxY = FLT_MIN;
    for (const auto& vertex : _vertices) {
        minY = std::min(minY, vertex.Position.y);
        maxY = std::max(maxY, vertex.Position.y);
    }
    _height = maxY - minY;

    // Define vertex attributes
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Position));
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Color));
    glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Normal));
    glVertexAttribPointer(3, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Uv));

    // Enable vertex attributes
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glEnableVertexAttribArray(2);
    glEnableVertexAttribArray(3);

    // Set the element count
    _elementCount = static_cast<uint32_t>(_indices.size());
}

void Mesh::Draw()
{
    // Bind the vertex array object
    glBindVertexArray(_vertexArrayObject);

    // Perform the draw call
    glDrawElements(GL_TRIANGLES, _elementCount, GL_UNSIGNED_INT, nullptr);
}
