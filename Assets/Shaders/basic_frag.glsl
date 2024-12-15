#version 450 core

in vec3 fragPos;      // Position from vertex shader
in vec3 fragNormal;   // Normal from vertex shader
in vec2 fragTexCoord; // Texture coordinates from vertex shader

out vec4 FragColor; // Final color output

uniform sampler2D texture1; // Diffuse texture

uniform vec3 lightPos;   // Light position in world space
uniform vec3 viewPos;    // Camera/view position
uniform vec3 lightColor; // Light color

void main() {
    // Ambient lighting
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    // Diffuse lighting
    vec3 norm = normalize(fragNormal);
    vec3 lightDir = normalize(lightPos - fragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    // Specular lighting
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - fragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * lightColor;

    // Combine results
    vec3 result = (ambient + diffuse + specular) * texture(texture1, fragTexCoord).rgb;
    FragColor = vec4(result, 1.0);
}
