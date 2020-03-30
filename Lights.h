#pragma once

#include <DirectXMath.h>

struct DirectionalLight {
	DirectX::XMFLOAT3 AmbientColor;
	float padding1;
	DirectX::XMFLOAT3 DiffuseColor;
	float padding2;
	DirectX::XMFLOAT3 Direction;
};

struct PointLight {
	DirectX::XMFLOAT3 AmbientColor;
	float padding1;
	DirectX::XMFLOAT3 DiffuseColor;
	float padding2;
	DirectX::XMFLOAT3 Position;
};
