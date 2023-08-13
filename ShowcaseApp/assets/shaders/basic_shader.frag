#version 330 core

out vec4 FragColor;  // Fragment shader output color
in vec4 vertexColor; // Interpolated vertex color
in vec2 texCoord;    // Interpolated texture coordinates (UV)
in vec2 InterpolatedTexCoord; // Corrected texture coordinates for blending

uniform sampler2D tex0; // 2D texture sampler GL_TEXTURE0
uniform sampler2D tex1; // 2D texture sampler GL_TEXTURE1

void main() {
    // Flip the texture coordinates vertically
    vec2 flippedTexCoord = vec2(InterpolatedTexCoord.x, 1.0 - InterpolatedTexCoord.y);

    // Sample textures from GL_TEXTURE0 and GL_TEXTURE1
    vec4 textureColor0 = texture(tex0, flippedTexCoord);
    vec4 textureColor1 = texture(tex1, flippedTexCoord);

    // Mix the two textures with a blending factor of 0.4
    FragColor = mix(textureColor0, textureColor1, 0.4) * vertexColor;
    // Set the output fragment color by applying the texture blending effect to the interpolated vertex color.
}