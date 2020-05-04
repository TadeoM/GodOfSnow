#include "GameEntity.h"

using namespace DirectX;

GameEntity::GameEntity(Mesh* mesh, Material* material, bool isFloor, DirectX::XMFLOAT3 playerPos)
{
	// Save the mesh
	this->mesh = mesh;
	this->material = material;
	this->isFloor = isFloor;
	if (this->isFloor) {
		playerPosition = playerPos;
	}
}

GameEntity::~GameEntity() {
}

Mesh* GameEntity::GetMesh() { return mesh; }
Transform* GameEntity::GetTransform() { return &transform; }
Material* GameEntity::GetMaterial() { return material; }

void GameEntity::UpdatePlayPosHistory(DirectX::XMFLOAT3 playPos)
{
	if (isFloor) {
		system("cls");
		//printf("%f %f %f\n", playPosHistory[0].x, playPosHistory[0].y, playPosHistory[0].z);
		playPosHistory[0].x = playPos.x;
		playPosHistory[0].y = 0;
		playPosHistory[0].z = playPos.z;
		for (int i = 49; i >= 1; i--)
		{
			playPosHistory[i] = playPosHistory[i-1];
		}
		
		playerPosition = playPos;
	}
}


void GameEntity::Draw(Microsoft::WRL::ComPtr<ID3D11DeviceContext> context, Camera* camera)
{
	SimpleVertexShader* vs = material->GetVertexShader();
	vs->SetFloat4("colorTint", material->GetColorTint());
	//vs->SetFloat3("decalPosition", playerPosition);
	
	vs->SetMatrix4x4("world", transform.GetWorldMatrix());
	vs->SetMatrix4x4("view", camera->GetView());
	vs->SetMatrix4x4("proj", camera->GetProjection());
	if (isFloor) {
		vs->SetFloat3("decalPosition", playerPosition);
		vs->SetData("decalHistory", &playPosHistory, sizeof(playPosHistory));
	}
	

	vs->SetInt("isFloor", isFloor);
	vs->CopyAllBufferData();

	// Draw the mesh
	mesh->SetBuffersAndDraw(context);
}
