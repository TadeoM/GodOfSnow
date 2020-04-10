
#include "ShaderIncludes.hlsli"

// The variables defined in this cbuffer will pull their data from the 
// constant buffer (ID3D11Buffer) bound to "vertex shader constant buffer slot 0"
// It was bound using context->VSSetConstantBuffers() over in C++.
cbuffer ExternalData : register(b0)
{
	float4 colorTint;
	float4 decalPosition;
	matrix world;
	matrix view;
	matrix proj;
}

// --------------------------------------------------------
// The entry point (main method) for our vertex shader
// --------------------------------------------------------
VertexToPixelNormalMap main(VertexShaderInput input)
{
	// Set up output struct
	VertexToPixelNormalMap output;

	// Modifying the position using the provided transformation (world) matrix
	matrix wvp = mul(proj, mul(view, world));
	output.position = mul(wvp, float4(input.position, 1.0f));

	// Calculate the final world position of the vertex
	output.worldPos = mul(world, float4(input.position, 1.0f)).xyz;

	float4 diffInNormal = decalPosition - float4(output.worldPos, 1);

	output.worldPos += diffInNormal;

	// Modify the normal so its also in world space
	output.normal = mul((float3x3)world, input.normal);
	output.normal = normalize(output.normal);

	// Modify the tangent much like the normal
	output.tangent = mul((float3x3)world, input.tangent);
	output.tangent = normalize(output.tangent);

	// Tints the color before passing it through
	output.color = colorTint;
	output.uv = input.uv;

	// Whatever we return will make its way through the pipeline to the
	// next programmable stage we're using (the pixel shader for now)
	return output;
}