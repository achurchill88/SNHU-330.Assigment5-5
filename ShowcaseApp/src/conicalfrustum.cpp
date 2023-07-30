#include "conicalfrustum.h"
#include <cmath>
#include <random>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

ConicalFrustum::ConicalFrustum(float topRadius, float bottomRadius, float height, int sectors)
        : topRadius(topRadius), bottomRadius(bottomRadius), height(height), sectors(sectors)
{
    generateVertices();
    generateIndices();
}

Mesh ConicalFrustum::GetMesh()
{
    return Mesh(vertices, indices);
}

std::vector<Vertex> ConicalFrustum::GetVertices()
{
    return vertices;
}

std::vector<unsigned int> ConicalFrustum::GetIndices()
{
    return indices;
}

void ConicalFrustum::generateVertices()
{
    const float sectorStep = 2.0f * M_PI / sectors;

    // Generate top vertex
    Vertex topVertex;
    topVertex.Position = glm::vec3(0.0f, height / 2.0f, 0.0f);
    topVertex.Normal = glm::vec3(0.0f, 1.0f, 0.0f);
    topVertex.Uv = glm::vec2(0.5f, 0.5f);
    vertices.push_back(topVertex);

    // Generate bottom vertex
    Vertex bottomVertex;
    bottomVertex.Position = glm::vec3(0.0f, -height / 2.0f, 0.0f);
    bottomVertex.Normal = glm::vec3(0.0f, -1.0f, 0.0f);
    bottomVertex.Uv = glm::vec2(6.f, 6.f);
    vertices.push_back(bottomVertex);

    // Generate side vertices
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<float> colorDistribution(0.0f, 1.0f);

    for (int i = 0; i <= sectors; ++i)
    {
        const float sectorAngle = i * sectorStep;
        const float xTop = topRadius * cosf(sectorAngle);
        const float zTop = topRadius * sinf(sectorAngle);
        const float xBottom = bottomRadius * cosf(sectorAngle);
        const float zBottom = bottomRadius * sinf(sectorAngle);

        Vertex sideVertexTop;
        sideVertexTop.Position = glm::vec3(xTop, height / 2.0f, zTop);
        sideVertexTop.Normal = glm::normalize(glm::vec3(xTop, (topRadius - bottomRadius) / height, zTop));
        sideVertexTop.Uv = glm::vec2(static_cast<float>(i) / sectors, 1.0f);
        //sideVertexTop.Color = glm::vec3(colorDistribution(gen), colorDistribution(gen), colorDistribution(gen));
        vertices.push_back(sideVertexTop);

        Vertex sideVertexBottom;
        sideVertexBottom.Position = glm::vec3(xBottom, -height / 2.0f, zBottom);
        sideVertexBottom.Normal = glm::normalize(glm::vec3(xBottom, (topRadius - bottomRadius) / height, zBottom));
        sideVertexBottom.Uv = glm::vec2(static_cast<float>(i) / sectors, 0.0f);
        //sideVertexBottom.Color = glm::vec3(colorDistribution(gen), colorDistribution(gen), colorDistribution(gen));
        vertices.push_back(sideVertexBottom);
    }
}

void ConicalFrustum::generateIndices()
{
    const int topVertexIndex = 0;
    const int bottomVertexIndex = 1;

    // Generate top indices
    for (int i = 0; i < sectors; ++i)
    {
        const int currentTop = 2 * (i + 2);
        const int nextTop = 2 * ((i + 1) % sectors + 2);
        const int currentBottom = currentTop + 1;
        const int nextBottom = nextTop + 1;

        // Generate top triangle
        indices.push_back(topVertexIndex);
        indices.push_back(currentTop);
        indices.push_back(nextTop);

        // Generate bottom triangle
        indices.push_back(bottomVertexIndex);
        indices.push_back(currentBottom);
        indices.push_back(nextBottom);

        // Generate side triangles
        indices.push_back(currentTop);
        indices.push_back(currentBottom);
        indices.push_back(nextTop);

        indices.push_back(currentBottom);
        indices.push_back(nextBottom);
        indices.push_back(nextTop);
    }
}
