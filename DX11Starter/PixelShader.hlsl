struct DirectionalLight
{
	float4 AmbientColor;
	float4 DiffuseColor;
	float3 Direction;
};

cbuffer externalData : register(b0)
{
	DirectionalLight light1;
	DirectionalLight light2;
}
// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
	float2 uv			: TEXCOORD;
};

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{
	float3 directionToLight1 = normalize(-light1.Direction);

	input.normal = normalize(input.normal);


	float dirNdotL1 = dot(directionToLight1, input.normal);
	dirNdotL1 = saturate(dirNdotL1);


	float3 finalDirColor1 = light1.DiffuseColor * dirNdotL1 + light1.AmbientColor;



	float3 directionToLight2 = normalize(-light2.Direction);


	float dirNdotL2 = dot(directionToLight2, input.normal);
	dirNdotL2 = saturate(dirNdotL2);


	float3 finalDirColor2 = light2.DiffuseColor * dirNdotL2 + light2.AmbientColor;

	// Just return the input color
	// - This color (like most values passing through the rasterizer) is 
	//   interpolated for each pixel between the corresponding vertices 
	//   of the triangle we're rendering
	return float4(finalDirColor1 + finalDirColor2, 1);
}