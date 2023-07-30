#version 330 core

out vec4 FragColor;  // Fragment shader output color
in vec4 vertexColor; // Interpolated vertex color
in vec2 texCoord;    // Interpolated texture coordinates (UV)

uniform sampler2D tex0; // 2D texture sampler  GL_TEXTURE0
uniform sampler2D tex1; // 2D texture sampler GL_TEXTURE1

void main() {
    vec2 flippedTexCoord = vec2(texCoord.x, 1.0 - texCoord.y); // Flip the texture coordinates vertically
    FragColor = mix(texture(tex0, flippedTexCoord), texture(tex1, flippedTexCoord), 0.4) * vertexColor;
    // Set the output fragment color by sampling the texture using interpolated UV coordinates.
}
