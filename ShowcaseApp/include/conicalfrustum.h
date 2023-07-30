#pragma once

#include <mesh.h>
#include <glm/glm.hpp>
#include <vector>

class ConicalFrustum
{
public:
    ConicalFrustum(float topRadius, float bottomRadius, float height, int sectors);  // Constructor with parameters

    Mesh GetMesh();  // Function to get the mesh representation of the conical frustum
    std::vector<Vertex> GetVertices();  // Function to get the vertices of the conical frustum
    std::vector<unsigned int> GetIndices();  // Function to get the indices of the conical frustum

private:
    std::vector<Vertex> vertices;  // Vector to store the vertices of the conical frustum
    std::vector<unsigned int> indices;  // Vector to store the indices of the conical frustum

    float topRadius;  // Top radius of the conical frustum
    float bottomRadius;  // Bottom radius of the conical frustum
    float height;  // Height of the conical frustum
    int sectors;  // Number of sectors used for generating the conical frustum

    void generateVertices();  // Function to generate the vertices of the conical frustum
    void generateIndices();  // Function to generate the indices of the conical frustum
};
