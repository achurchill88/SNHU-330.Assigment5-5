#pragma once

#include <mesh.h>
#include <glm/glm.hpp>
#include <vector>

class Cylinder
{
public:
    Cylinder(float radius, float height, int sectors);  // Constructor with parameters

    Mesh GetMesh();  // Function to get the mesh representation of the cylinder
    std::vector<Vertex> GetVertices();  // Function to get the vertices of the cylinder
    std::vector<unsigned int> GetIndices();  // Function to get the indices of the cylinder

private:
    std::vector<Vertex> vertices;  // Vector to store the vertices of the cylinder
    std::vector<unsigned int> indices;  // Vector to store the indices of the cylinder

    float radius;  // Radius of the cylinder
    float height;  // Height of the cylinder
    int sectors;  // Number of sectors used for generating the cylinder

    void generateVertices();  // Function to generate the vertices of the cylinder
    void generateIndices();  // Function to generate the indices of the cylinder
};
