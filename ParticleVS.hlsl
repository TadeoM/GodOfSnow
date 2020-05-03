
// Constant buffer for C++ data being passed in
cbuffer externalData : register(b0)
{
    matrix view;
    matrix projection;
};

// Describes individual vertex data
struct VertexShaderInput
{
    float3 position		: POSITION;
    float2 uv			: TEXCOORD;
    float4 color		: COLOR;
};

// Defines the output data of our vertex shader
struct VertexToPixel
{
    float4 position		: SV_POSITION;
    float2 uv           : TEXCOORD0;
	float4 color		: TEXCOORD1;
};

// The entry point for our vertex shader
VertexToPixel main(VertexShaderInput input)
{
    // Set up output
    VertexToPixel output;

    // Calculate output position
    matrix viewProj = mul(projection, view);
    output.position = mul(viewProj, float4(input.position, 1.0f));

	// Pass uv through
	output.uv = input.uv;
	output.color = input.color;
   
    return output;
}