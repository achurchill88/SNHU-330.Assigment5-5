//
// Created by churc on 7/12/2023.
//

#pragma once
#include <glm/glm.hpp>
#include <vector>

struct Vertex{
    glm::vec3 Position {0.f, 0.f, 0.f};
    glm::vec3 Color {1.f, 1.f, 1.f};
    glm::vec3 Normal {0.f, 0.f, 0.f};
    glm::vec2 Uv {1.f, 1.f};
};

struct Shapes {

    static inline std::vector<Vertex> planeVertices {
            // Plane vertices (positions and colors)
            { .Position = {-0.5f, 0.0f, -0.5f}, .Color = {0.0f, 1.0f, 0.0f} }, // Bottom-left, green
            { .Position = {0.5f, 0.0f, -0.5f}, .Color = {0.0f, 1.0f, 0.0f} },  // Bottom-right, green
            { .Position = {0.5f, 0.0f, 0.5f}, .Color = {0.0f, 1.0f, 0.0f} },   // Top-right, green
            { .Position = {-0.5f, 0.0f, 0.5f}, .Color = {0.0f, 1.0f, 0.0f} }   // Top-left, green
    };

    static inline std::vector<uint32_t> planeElements {
            // Plane indices to form two triangles
            0, 1, 2, // First triangle
            2, 3, 0  // Second triangle
    };

    static inline std::vector<Vertex> cubeVertices {
            // Cube vertices (positions and colors)
            { .Position = {-0.5f, -0.5f, 0.5f}, .Color = {1.f, 0.f, 1.f} }, // Front-bottom-left, pink
            { .Position = {0.5f, -0.5f, 0.5f}, .Color = {1.f, 1.f, 0.f} },  // Front-bottom-right, yellow
            { .Position = {0.5f, 0.5f, 0.5f}, .Color = {0.5f, 0.f, 0.5f} }, // Front-top-right, purple
            { .Position = {-0.5f, 0.5f, 0.5f}, .Color = {0.f, 0.f, 1.f} },  // Front-top-left, blue
            // Back face vertices with different colors
            { .Position = {-0.5f, -0.5f, -0.5f}, .Color = {0.f, 1.f, 1.f} }, // Back-bottom-left, cyan
            { .Position = {0.5f, -0.5f, -0.5f}, .Color = {0.f, 1.f, 0.f} },  // Back-bottom-right, green
            { .Position = {0.5f, 0.5f, -0.5f}, .Color = {1.f, 0.f, 0.f} },  // Back-top-right, red
            { .Position = {-0.5f, 0.5f, -0.5f}, .Color = {0.f, 0.5f, 0.5f} } // Back-top-left, teal
    };

    static inline std::vector<uint32_t> cubeElements {
            // Cube indices for front, back, and connecting faces
            0, 1, 2,
            2, 3, 0,
            4, 5, 6,
            6, 7, 4,
            0, 4, 1,
            1, 4, 5,
            1, 5, 2,
            2, 5, 6,
            2, 6, 3,
            3, 6, 7,
            3, 7, 0,
            0, 7, 4
    };

    static inline std::vector<Vertex> pyramidVertices {
            // Pyramid vertices (positions, colors, and UVs)
            { .Position = {-0.5f, -0.5f, 0.5f}, .Color = {1.f, 0.f, 1.f}, .Uv = {0.f, 0.f} }, // Front-bottom-left, pink
            { .Position = {0.5f, -0.5f, 0.5f}, .Color = {1.f, 1.f, 0.f}, .Uv = {1.f, 0.f} },  // Front-bottom-right, yellow
            { .Position = {0.5f, -0.5f, -0.5f}, .Color = {0.5f, 0.f, 0.5f}, .Uv = {1.f, 1.f} }, // Back-bottom-right, purple
            { .Position = {-0.5f, -0.5f, -0.5f}, .Color = {0.f, 0.f, 1.f}, .Uv = {0.f, 1.f} }, // Back-bottom-left, blue
            // Apex vertex with white color and center UV
            { .Position = {0.f, 0.5f, 0.f}, .Color = {1.f, 1.f, 1.f}, .Uv = {0.5f, 0.5f} }
    };

    static inline std::vector<uint32_t> pyramidElements {
            // Pyramid indices for the base and side faces
            0, 1, 2,
            2, 3, 0,
            0, 4, 1,
            1, 4, 2,
            2, 4, 3,
            3, 4, 0
    };
};
