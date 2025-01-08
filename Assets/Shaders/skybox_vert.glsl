#version 450 core

layout (location = 0) in vec3 aPosition;
layout (location = 1) in vec3 aNormal; // Vertex normal attribute
layout (location = 2) in vec3 aTangent; // Vertex tangent attribute
layout (location = 3) in vec2 aTexCoord; // Vertex color attribute

out vec3 g_texCoord; // Pass the texcoord to the fragment shader

uniform mat4 uView; // View (camera) matrix
uniform mat4 uProjection; // Projection matrix

void main(void)
{
	g_texCoord = aPosition;
	gl_Position = uProjection * uView * vec4(aPosition, 1.0);
}
