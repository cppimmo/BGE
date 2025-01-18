#version 450 core

layout (location = 0) out vec4 outColor;

in vec3 g_texCoord;

uniform samplerCube uSamplerCube;

void main(void)
{
	outColor = texture(uSamplerCube, g_texCoord); // Output the sampled color
}
