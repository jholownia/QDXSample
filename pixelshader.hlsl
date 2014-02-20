#include "cbuffers.hlsli"

float4 LightPixelShader(PixelInput input) : SV_TARGET
{
	// Sample the pixel color from the texture using the sampler at this texture coordinate location.
	float4 textureColor = shaderTextures[0].Sample(samplerType, input.tex);

	// Sample the bump map
	float4 bumpMap = shaderTextures[1].Sample(samplerType, input.tex);

	// Expand the range of normal value to (-1,1)
	bumpMap = (bumpMap * 2.0f) - 1.0f;

	// Calculate the normal from the data in bump map
	float3 bumpNormal = input.normal + bumpMap.x * input.tangent + bumpMap.y * input.binormal;

	// Normalize bump normal
	bumpNormal = normalize(bumpNormal);

	// Set default output color to ambient light value for all pixels
	float4 color = ambientColor;

	// Invert the light direction for calculations.
    float3 lightDir = -lightDirection;

    // Calculate the amount of light on this pixel.
    float lightIntensity = saturate(dot(bumpNormal, lightDir));

	// Determine final diffuse color
	color += (diffuseColor * lightIntensity);

	// Combine the final bump light color with the texture color
	color = color * textureColor;

	// Initialize specular
	float4 specular = float4(0.0f, 0.0f, 0.0f, 0.0f);	

	if (lightIntensity > 0.0f)
	{
		// Sample the pixel from specular map texture
		float4 specularMap = shaderTextures[2].Sample(samplerType, input.tex);
		
		// Calculate the reflection vector based on the light intensity, normal vector and light direction		
		float3 reflection = normalize(2 * lightIntensity * bumpNormal - lightDir);

		// Determine the amount of specular light based on the reflection vector, viewing direction and specular power		
		specular = specularIntensity * specularMap * specularColor * pow(saturate(dot(reflection, input.viewDirection)), specularPower);

		// Add the specular component
		color = saturate(color + specular);
	}	

    return color;
}