#version 450 core

layout (location = 0) in vec3 aPosition; // Vertex position attribute
layout (location = 1) in vec3 aNormal; // Vertex normal attribute
layout (location = 2) in vec3 aTangent; // Vertex tangent attribute
layout (location = 3) in vec2 aTexCoord; // Vertex color attribute

out vec2 g_texCoord;
out vec4 g_vertexColor; // Pass the color to the fragment shader
out float g_viewSpaceZ; // Distance in view space for fog calculation

// Uniform matrices
//struct ModelViewProjection
//{
//	mat4 model;
//	mat4 view;
//	mat4 projection;
//};
uniform mat4 uModel;      // Model matrix
uniform mat4 uView;       // View (camera) matrix
uniform mat4 uProjection; // Projection matrix

void main(void)
{
	g_texCoord = aTexCoord;
	g_vertexColor = vec4(1.0, 0.5, 0.0, 1.0); // Pass color to fragment shader as is

	// Calculate view space position
	vec4 viewSpacePosition = uView * uModel * vec4(aPosition, 1.0);
	g_viewSpaceZ = -viewSpacePosition.z; // Positive distance from the camera

	// Transform the vertex position by the Model-View-Projection (MVP) matrix
	gl_Position = uProjection * viewSpacePosition;
}
