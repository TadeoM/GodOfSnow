#ifndef __GGP_SHADER_INCLUDES__
#define __GGP_SHADER_INCLUDES__

//ALL STUFF GOES HERE

struct DirectionalLight {
	float3 AmbientColor;
	float3 DiffuseColor;
	float3 Direction;
};
struct PointLight {
	float3 AmbientColor;
	float3 DiffuseColor;
	float3 Position;
};


float Diffuse(float3 normal, float3 lightDir)
{
	return saturate(dot(normal, -normalize(lightDir)));
}

float SpecularPhong(float3 normal, float3 lightDir, float3 toCamera, float specExp)
{
	// Calculate light reflection vector
	float3 refl = reflect(lightDir, normal);

	return pow(saturate(dot(refl, toCamera)), specExp);
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
	float4 position		: SV_POSITION;	// XYZW position (System Value Position)
	float4 color		: COLOR;        // RGBA color
	float2 uv			: TEXCOORD;
	float3 normal		: NORMAL;
	float3 worldPos		: POSITION;
};

//Struct representing a single vertex worth of data
struct VertexShaderInput
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float3 position		: POSITION;     // XYZ position
	float3 normal		: NORMAL;		// normal
	float2 uv			: TEXCOORD;		// uv
	float3 tangent		: TANGENT;		// tangent
};

struct VertexToPixelNormalMap
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;	// XYZW position (System Value Position)
	float4 color		: COLOR;        // RGBA color
	float2 uv			: TEXCOORD;
	float3 normal		: NORMAL;
	float3 worldPos		: POSITION;
	float3 tangent		: TANGENT;
};

#endif