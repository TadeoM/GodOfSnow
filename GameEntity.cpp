#include "GameEntity.h"

using namespace DirectX;

GameEntity::GameEntity(Mesh* mesh, Material* material)
{
	// Save the mesh
	this->mesh = mesh;
	this->material = material;
}

Mesh* GameEntity::GetMesh() { return mesh; }
Transform* GameEntity::GetTransform() { return &transform; }
Material* GameEntity::GetMaterial() { return material; }


void GameEntity::Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Camera* camera)
{
	SimpleVertexShader* vs = material->GetVertexShader();
	vs->SetFloat4("colorTint", material->GetColorTint());
	vs->SetMatrix4x4("world", transform.GetWorldMatrix());
	vs->SetMatrix4x4("view", camera->GetView());
	vs->SetMatrix4x4("proj", camera->GetProjection());
	vs->CopyAllBufferData();

	// Draw the mesh
	mesh->SetBuffersAndDraw(context);
}
