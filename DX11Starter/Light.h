#pragma once
#include "DXCore.h"
#include <DirectXMath.h>
struct Light
{
	DirectX::XMFLOAT4 AmbientColor;
	DirectX::XMFLOAT4 DiffuseColor;
	DirectX::XMFLOAT3 Direction;

};