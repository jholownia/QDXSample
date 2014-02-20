#include "cbuffers.hlsli"

PixelInput LightVertexShader(VertexInput input)
{
	PixelInput output;

	float4 worldPosition;

	input.position.w = 1.0f;

	output.position = mul(input.position, worldMatrix);
	output.position = mul(output.position, viewMatrix);
	output.position = mul(output.position, projectionMatrix);

	output.tex = input.tex;

	// Calculate the normal vector against the world matrix only.
    output.normal = mul(input.normal, (float3x3)worldMatrix);
	
    // Normalize the normal vector.
    output.normal = normalize(output.normal);

	// Calculate tangent vector
	output.tangent = mul(input.tangent, (float3x3)worldMatrix);
	output.tangent = normalize(output.tangent);

	// Calculate binormal vector
	output.binormal = mul(input.binormal, (float3x3)worldMatrix);
	output.binormal = normalize(output.binormal);

	// Calculate the position of the vertex in the world
	worldPosition = mul(input.position, worldMatrix);

	// Determine the vieweing direction based on the position of the camera and the position of the vertex
	output.viewDirection = cameraPosition.xyz - worldPosition.xyz;

	// Normalize the viewing direction vector
	output.viewDirection = normalize(output.viewDirection);

	return output;
}