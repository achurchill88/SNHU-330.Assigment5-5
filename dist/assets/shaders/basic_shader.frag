#version 330 core

out vec4 FragColor;  // Fragment shader output color
in vec4 vertexColor; // Interpolated vertex color
in vec2 texCoord;    // Interpolated texture coordinates (UV)

uniform sampler2D tex; // 2D texture sampler

void main() {
    FragColor = texture(tex, texCoord) * vertexColor;
    // Set the output fragment color by sampling the texture using interpolated UV coordinates.
}

