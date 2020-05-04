#pragma once

#include <wrl/client.h>
#include <DirectXMath.h>
#include "Mesh.h"
#include "Transform.h"
#include "Camera.h"
#include "Material.h"

class GameEntity
{
public:
	GameEntity(Mesh* mesh, Material* material, bool isFloor, DirectX::XMFLOAT3 playerPos);
	~GameEntity();
	Mesh* GetMesh();
	Transform* GetTransform();
	Material* GetMaterial();
	void UpdatePlayPosHistory(DirectX::XMFLOAT3 playPos);

	void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Camera* camera);

private:
	Mesh* mesh;
	Transform transform;
	Material* material;
	DirectX::XMFLOAT4 playPosHistory[50];
	bool isFloor;
	DirectX::XMFLOAT3 playerPosition;
};

