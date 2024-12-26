#version 420 compatibility

layout (location = 0) out vec4 outColor;

in vec3 g_vertexColor;

void main(void)
{
	outColor = vec4(g_vertexColor, 1.0);
}
