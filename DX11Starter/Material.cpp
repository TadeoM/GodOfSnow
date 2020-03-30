#include "Material.h"





Material::Material(SimplePixelShader* ps, SimpleVertexShader * vs)
{
	pixelShader = ps;
	vertexShader = vs;
}

Material::~Material()
{
}

SimplePixelShader * Material::GetPixelShader()
{
	return pixelShader;
}

SimpleVertexShader * Material::GetVertexShader()
{
	return vertexShader;
}
