#pragma once

#include "GameEntity.h"

class PlayerBall : public GameEntity 
{
public:
	PlayerBall(Mesh* mesh, Material* material, bool isFloor, DirectX::XMFLOAT4 playerPos, float mouseLookSpeed);
	
	void Update(float dt, HWND windowHandle);
	
	Transform* GetTransform();
	
private:
	float mouseLookSpeed;
	POINT prevMousePosition;
	Transform transform;
};