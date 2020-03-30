#pragma once
#include <wrl/client.h>
#include <DirectXMath.h>
#include <d3d11.h>
#include "SimpleShader.h"

class Material
{

public:
	Material(SimplePixelShader* pixelShader, SimpleVertexShader* vertexShader, DirectX::XMFLOAT4 colorTint, float shininess, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> diffuseTexture, Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerOptions, Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> normalMap);
	~Material();

	SimplePixelShader* GetPixelShader() { return pixelShader; };
	SimpleVertexShader* GetVertexShader() {return vertexShader; };
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetDiffuseTexture() { return diffuseTexture; };
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> GetNormalMap() { return normalMap; };
	Microsoft::WRL::ComPtr<ID3D11SamplerState> GetSamplerOptions() { return samplerOptions; };
	DirectX::XMFLOAT4 GetColorTint() { return colorTint; };
	float GetShiny() { return shininess; };

	void SetColorTint(DirectX::XMFLOAT4 tint);

private:

	DirectX::XMFLOAT4 colorTint;

	float shininess;

	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> diffuseTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> normalMap;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerOptions;
	SimplePixelShader* pixelShader;
	SimpleVertexShader* vertexShader;
};

