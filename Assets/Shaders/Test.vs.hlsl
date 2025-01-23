
cbuffer CB_WorldViewProjection : register(b0)
{
    float4x4 g_world;
    float4x4 g_view;
    float4x4 g_projection;
};

struct VSInput
{
	float3 position: POSITION;
	float3 color: COLOR0;
};

struct VSOutput
{
	float4 position: SV_Position;
	float3 color: COLOR0;
};

VSOutput Main(VSInput input)
{
	VSOutput output = (VSOutput)0;
    float4 worldPosition = mul(float4(input.position, 1.0), g_world); // Apply world matrix
    float4 viewPosition = mul(worldPosition, g_view); // Apply view matrix
    float4 projPosition = mul(viewPosition, g_projection); // Apply projection matrix
	//output.position = float4(input.position, 1.0);
    output.position = projPosition; // Set transformed position
	output.color = input.color; // Pass through color
	return output;
}
