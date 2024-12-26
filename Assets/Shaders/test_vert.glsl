#version 420 compatibility

layout (location = 0) in vec3 inPosition;
layout (location = 1) in vec3 inColor;

out vec3 g_vertexColor;

void main(void)
{
	gl_Position = vec4(inPosition, 1.0);
	g_vertexColor = inColor;
}
