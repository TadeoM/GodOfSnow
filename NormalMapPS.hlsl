#include "ShaderIncludes.hlsli"

// The variables defined in this cbuffer will pull their data from the 
// constant buffer (ID3D11Buffer) bound to "vertex shader constant buffer slot 0"
// It was bound using context->VSSetConstantBuffers() over in C++.
cbuffer externalData : register(b0)
{
	DirectionalLight dLight1;
	DirectionalLight dLight2;
	DirectionalLight dLight3;

	PointLight pLight1;

	float3 cameraPosition;

	float material1Shiny;
	float material2Shiny;
	float material3Shiny;
}

// Texture-related resources
Texture2D diffuseTexture	: register(t0);
Texture2D normalMap			: register(t1);
SamplerState samplerOptions : register(s0);


// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixelNormalMap input) : SV_TARGET
{

	// sample texture to get a color
	float3 surfaceColor = diffuseTexture.Sample(samplerOptions, input.uv).rgb;

	// grab the normal map sample and unpack the normal
	float3 unpackedNormal = normalMap.Sample(samplerOptions, input.uv).rgb * 2 - 1;

	float3 N = input.normal;
	float3 T = input.tangent;
	T = normalize(T - N * dot(T, N));
	float3 B = cross(T, N);
	float3x3 TBN = float3x3(T, B, N);

	// Adjust and overwrite the existing normal
	input.normal = normalize(mul(unpackedNormal, TBN));

	// Calculate the vector from the pixel's world position to the camera
	float3 toCamera = normalize(cameraPosition - input.worldPos);

	// directional light 1 --------------------------------------------------------
	float3 directionToLight1 = normalize(dLight1.Direction);

	float dirNdotL1 = Diffuse(input.normal, directionToLight1);

	float dir_spec1 = SpecularPhong(input.normal, directionToLight1, toCamera, 64.0f);
	dir_spec1 *= any(dirNdotL1);

	float3 finalDirColor1 = dLight1.DiffuseColor * dirNdotL1 + dir_spec1;

	// directional light 2 --------------------------------------------------------
	float3 directionToLight2 = normalize(dLight2.Direction);

	float dirNdotL2 = Diffuse(input.normal, directionToLight2);

	float3 finalDirColor2 = dLight2.DiffuseColor * dirNdotL2 + dLight2.AmbientColor * input.color;

	// directional light 3 --------------------------------------------------------
	float3 directionToLight3 = normalize(dLight3.Direction);

	float dirNdotL3 = Diffuse(input.normal, directionToLight3);

	float3 finalDirColor3 = dLight3.DiffuseColor * dirNdotL3 + dLight3.AmbientColor * input.color;

	// point light 1	   --------------------------------------------------------
	float3 pointLightDirection = normalize(input.worldPos - pLight1.Position);

	float pDiffuse1 = Diffuse(input.normal, pointLightDirection);

	float point_spec1 = SpecularPhong(input.normal, pointLightDirection, toCamera, 256.0f);
	point_spec1 *= any(pDiffuse1);

	float3 finalPLColor1 = pLight1.DiffuseColor * pDiffuse1 + point_spec1;

	// total light
	float3 totalLight = finalPLColor1 + pLight1.AmbientColor + (finalDirColor1);

	// just the point light for now
	return float4(totalLight * input.color * surfaceColor, 1);
	//just add the three values together
	return float4(finalDirColor1 + finalDirColor2 + finalDirColor3, 1);
}