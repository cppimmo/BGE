#version 450 core

layout (location = 0) out vec4 outColor;

in vec2 g_texCoord;
in vec4 g_vertexColor; // Interpolated color from the vertex shader
in float g_viewSpaceZ;

layout (binding = 0) uniform sampler2D uSampler2D;

// Fog parameters
uniform vec3 uFogColor = vec3(0.5, 0.6, 0.7);
uniform float uFogDensity = 0.1;
uniform float uFogStart = 10.0;
uniform float uFogEnd = 50.0;
uniform int uFogType = 1;

void main(void)
{
	vec4 texColor = texture(uSampler2D, g_texCoord);

	float fogFactor = 1.0;
	if (uFogType == 0)
	{
		fogFactor = clamp((uFogEnd - g_viewSpaceZ) / (uFogEnd - uFogStart), 0.0, 1.0);
	}
	else if (uFogType == 1)
	{
		fogFactor = exp(-uFogDensity * g_viewSpaceZ);
	}

	vec3 finalColor = mix(uFogColor, texColor.rgb, fogFactor);

	outColor = vec4(finalColor, texColor.a);

	//outColor = g_vertexColor; // Output the color
	//outColor = texture(uSampler2D, g_texCoord);
}
