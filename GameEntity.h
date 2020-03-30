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
	GameEntity(Mesh* mesh, Material* material);

	Mesh* GetMesh();
	Transform* GetTransform();
	Material* GetMaterial();

	void Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Camera* camera);

private:

	Mesh* mesh;
	Transform transform;
	Material* material;
};

