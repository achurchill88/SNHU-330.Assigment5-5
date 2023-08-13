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
uniform vec3 lightPosition1; // Position of the key light
uniform vec3 lightPosition2; // Position of the fill light
uniform vec3 lightColor1;    // Color of the key light
uniform float lightIntensity1; // Intensity of the key light
uniform float lightIntensity2; // Intensity of the fill light

void main() {
    // Transform vertex position from object to clip space
    gl_Position = projection * view * model * vec4(position, 1.0);

    // Pass per-vertex color to fragment shader
    vertexColor = vec4(color, 1.0);

    // Pass texture coordinates (UV) to fragment shader
    texCoord = uv;

    // Corrected texture coordinate for blending
    InterpolatedTexCoord = vec2(uv.x, 1.0 - uv.y);

    // Calculate the light direction and intensity for the key light
    vec3 lightDirection1 = normalize(lightPosition1 - position);
    float diffuse1 = max(dot(normal, lightDirection1), 0.0);
    float ambient1 = 0.2; // A small ambient light for the key light

    // Calculate the light direction and intensity for the fill light
    vec3 lightDirection2 = normalize(lightPosition2 - position);
    float diffuse2 = max(dot(normal, lightDirection2), 0.0);
    float ambient2 = 0.0; // No ambient light for the fill light

    // Calculate the final color by blending the key light and fill light contributions
    vertexColor.rgb = vertexColor.rgb * (ambient1 + diffuse1 * lightIntensity1) + lightColor1 * (ambient2 + diffuse2 * lightIntensity2);
}
