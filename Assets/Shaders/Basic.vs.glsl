#version 450 core

layout(location = 0) in vec3 aPos;      // Vertex position
layout(location = 1) in vec3 aNormal;   // Vertex normal
layout(location = 2) in vec2 aTexCoord; // Texture coordinate

out vec3 fragPos;      // Position of the vertex in world space
out vec3 fragNormal;   // Normal of the vertex
out vec2 fragTexCoord; // Texture coordinate passed to fragment shader

uniform mat4 model;    // Model matrix
uniform mat4 view;     // View matrix
uniform mat4 projection; // Projection matrix

void main() {
    fragPos = vec3(model * vec4(aPos, 1.0));
    fragNormal = mat3(transpose(inverse(model))) * aNormal; // Correctly transform normal
    fragTexCoord = aTexCoord;
    gl_Position = projection * view * vec4(fragPos, 1.0);
}
