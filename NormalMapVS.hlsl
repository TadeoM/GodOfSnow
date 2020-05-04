
#include "ShaderIncludes.hlsli"

// The variables defined in this cbuffer will pull their data from the 
// constant buffer (ID3D11Buffer) bound to "vertex shader constant buffer slot 0"
// It was bound using context->VSSetConstantBuffers() over in C++.
cbuffer ExternalData : register(b0)
{
	float4 colorTint;
	matrix world;
	matrix view;
	matrix proj;
	float4 decalHistory[50];
	float3 decalPosition;
	int decalCount;
	int isFloor;
}

Texture2D indentTexture		: register(t2); //indent texture for sampling, register(t2) because its the same as it is in the PS
SamplerState samplerOptions : register(s0);
// --------------------------------------------------------
// The entry point (main method) for our vertex shader
// --------------------------------------------------------
VertexToPixelNormalMap main(VertexShaderInput input)
{
	// Set up output struct
	VertexToPixelNormalMap output;

	// Tints the color before passing it through
	output.color = colorTint;
	output.uv = input.uv;

	//sample the indent texture also mess with the input position
	//float4 indent = indentTexture.SampleLevel(samplerOptions, float4(input.uv.x, input.uv.y, 0, 0), 0);
	
	//indent = float4()
	//input.position.y = -(1.0f - indent.x) * 5.0f; //change the 5.0 to adjust how far it will indent

	// Modifying the position using the provided transformation (world) matrix
	matrix wvp = mul(proj, mul(view, world));
	output.position = mul(wvp, float4(input.position, 1.0f));

	// Calculate the final world position of the vertex
	output.worldPos = mul(world, float4(input.position, 1.0f)).xyz;
	if (isFloor == 1) {
		// get world and decal positions and change their y values
		float3 worldPos2D = output.worldPos;
		worldPos2D.y = 0;

		float radius = 3;

		float outputHeightChangeVal = 0;
		float outputColorChangeVal = 0;
		float3 decalPos2D = decalPosition;
		decalPos2D.y = 0;
		float distanceTest = distance(worldPos2D, decalPos2D);

		//if (distanceTest < radius && outputHeightChangeVal < radius - distanceTest) {
		//	outputColorChangeVal = ((radius - distanceTest) / radius) * .8;
		//	//outputColorChangeVal = 1;
		//	outputHeightChangeVal = radius - distanceTest;
		//}
		for (int i = 0; i < 49; i++) {

			float3 decalPos2D = decalHistory[i];
			decalPos2D.y = 0;
			float distanceTest = distance(worldPos2D, decalPos2D);

			if (distanceTest < radius && outputHeightChangeVal < radius - distanceTest) {
				outputColorChangeVal = ((radius - distanceTest) / radius) * .8;
				//outputColorChangeVal = 1;
				outputHeightChangeVal = radius - distanceTest;
			}
		}
		output.color -= outputColorChangeVal;
		output.position.y -= outputHeightChangeVal;
	}

	// Modify the normal so its also in world space
	output.normal = mul((float3x3)world, input.normal);
	output.normal = normalize(output.normal);

	// Modify the tangent much like the normal
	output.tangent = mul((float3x3)world, input.tangent);
	output.tangent = normalize(output.tangent);

	// Whatever we return will make its way through the pipeline to the
	// next programmable stage we're using (the pixel shader for now)
	return output;
}