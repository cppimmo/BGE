#version 420 compatibility

layout (location = 0) out vec4 outColor;

in vec4 g_vertexColor; // Interpolated color from the vertex shader

void main(void)
{
	outColor = g_vertexColor; // Output the color
}
