#pragma once
#include "SimpleShader.h"
class Material
{
public:
	Material(SimplePixelShader* ps, SimpleVertexShader* vs);
	~Material();

	SimplePixelShader* GetPixelShader();
	SimpleVertexShader* GetVertexShader();
	

private:
	SimplePixelShader* pixelShader;
	SimpleVertexShader* vertexShader;
};

