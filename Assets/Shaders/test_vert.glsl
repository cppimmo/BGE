#version 420 compatibility

layout (location = 0) in vec3 inPosition; // Vertex position attribute
layout (location = 1) in vec3 inColor; // Vertex color attribute

out vec3 g_vertexColor; // Pass the color to the fragment shader

// Uniform matrices
uniform mat4 uModel;      // Model matrix
uniform mat4 uView;       // View (camera) matrix
uniform mat4 uProjection; // Projection matrix

void main(void)
{
	// Transform the vertex position by the Model-View-Projection (MVP) matrix
	gl_Position = uProjection * uView * uModel * vec4(inPosition, 1.0);
	g_vertexColor = inColor; // Pass color to fragment shader as is
}
