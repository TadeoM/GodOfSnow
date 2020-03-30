#pragma once
#include "DXCore.h"
#include <DirectXMath.h>
#include "Mesh.h"
#include "Material.h"

class Entity
{
public:
	Entity(Material* mat,Mesh* m, float xpos, float ypos, float zpos, float xrot, float yrot, float zrot, float xscale, float yscale, float zscale);
	Entity(Material* mat, Mesh* m, DirectX::XMFLOAT3 pos, DirectX::XMFLOAT3 rot, DirectX::XMFLOAT3 scale);
	Entity(Material * mat, Mesh * m);
	~Entity();

	void SetPosition(DirectX::XMFLOAT3 pos);

	void SetPositionX(float x);

	void SetPositionY(float y);

	void SetPositionZ(float z);

	void SetRotation(DirectX::XMFLOAT3 rot);

	void SetRotationX(float x);

	void SetRotationY(float y);

	void SetRotationZ(float z);

	void SetScale(DirectX::XMFLOAT3 scale);

	void SetScaleX(float x);

	void SetScaleY(float y);

	void SetScaleZ(float z);

	DirectX::XMFLOAT3 GetScale();

	DirectX::XMFLOAT3 GetRotation();

	DirectX::XMFLOAT3 GetPosition();



	void Move(DirectX::XMFLOAT3 movement);
	void MoveX(float x);
	void MoveY(float y);
	void MoveZ(float z);
	void Rotate(DirectX::XMFLOAT3 rotationAmount);
	void RotateX(float x);
	void RotateY(float y);
	void RotateZ(float z);
	void Scale(DirectX::XMFLOAT3 scaleAmount);
	void ScaleX(float x);
	void ScaleY(float y);
	void ScaleZ(float z);

	DirectX::XMFLOAT4X4 UpdateWorld();

	void PrepareMaterials(DirectX::XMFLOAT4X4 view, DirectX::XMFLOAT4X4 proj);
	

protected:
	DirectX::XMFLOAT4X4 world; //World matrix
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 scale;
	Mesh* mesh;
	Material* material;
	
};

