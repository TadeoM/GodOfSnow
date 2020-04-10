#include "Material.h"

Material::Material(SimplePixelShader* pixelShader, SimpleVertexShader* vertexShader, 
	DirectX::XMFLOAT4 colorTint, float shininess, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> diffuseTexture, 
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerOptions, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> normalMap,
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> indentTexture)
{
	this->pixelShader = pixelShader;
	this->vertexShader = vertexShader;
	this->colorTint = colorTint;
	this->shininess = shininess;
	this->diffuseTexture = diffuseTexture;
	this->samplerOptions = samplerOptions;
	this->normalMap = normalMap;
	this->indentTexture = indentTexture;
}

Material::~Material()
{
}

void Material::SetColorTint(DirectX::XMFLOAT4 tint)
{
	this->colorTint = tint;
}
