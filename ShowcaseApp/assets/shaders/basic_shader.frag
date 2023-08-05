#version 330 core

out vec4 FragColor;  // Fragment shader output color
in vec4 vertexColor; // Interpolated vertex color
in vec2 texCoord;    // Interpolated texture coordinates (UV)
in vec3 FragNormal;  // Interpolated normal

uniform sampler2D tex0; // 2D texture sampler GL_TEXTURE0

// Lighting uniforms
uniform vec3 lightDirection;
uniform vec3 lightColor;
uniform vec3 pointLightPosition;
uniform vec3 pointLightColor;

// Point light attenuation
uniform float pointLightAttenuation;

void main() {
    vec2 flippedTexCoord = vec2(texCoord.x, 1.0 - texCoord.y); // Flip the texture coordinates vertically

    // Calculate diffuse lighting (Lambertian) for the key light
    vec3 lightDir = normalize(lightDirection);
    float diffuseFactor = max(dot(FragNormal, lightDir), 0.0);
    vec3 diffuseColor = texture(tex0, flippedTexCoord).rgb * diffuseFactor * lightColor;

    // Calculate diffuse lighting for the fill light
    vec3 fillLightDir = normalize(pointLightPosition - gl_FragCoord.xyz);
    float fillDiffuseFactor = max(dot(FragNormal, fillLightDir), 0.0);
    vec3 fillDiffuseColor = fillDiffuseFactor * pointLightColor * 0.1; // Fill light intensity is 10% of the key light

    // Combine diffuse lighting from key light and fill light
    vec3 finalColor = diffuseColor + fillDiffuseColor;

    // Apply point light attenuation
    finalColor *= pointLightAttenuation;

    FragColor = vec4(finalColor * vertexColor.rgb, 1.0);
}
