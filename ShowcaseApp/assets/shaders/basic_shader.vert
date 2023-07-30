#version 330 core

// Input attributes
layout (location = 0) in vec3 position;  // Vertex position
layout (location = 1) in vec3 color;     // Vertex color
layout (location = 2) in vec3 normal;    // Vertex normal
layout (location = 3) in vec2 uv;        // Texture coordinates (UV)

// Output variables
out vec4 vertexColor;  // Interpolated vertex color
out vec2 texCoord;     // Interpolated texture coordinates (UV)
out vec2 InterpolatedTexCoord; // Send the corrected texture coordinate to the fragment shader


// Uniform variables
uniform mat4 view;       // View matrix
uniform mat4 projection; // Projection matrix
uniform mat4 model;      // Model matrix

void main() {
    // Transform vertex position from object to clip space
    gl_Position = projection * view * model * vec4(position, 1.0);

    // Pass per-vertex color to fragment shader
    vertexColor = vec4(color, 1.0);

    // Pass texture coordinates (UV) to fragment shader
    texCoord = uv;
}
