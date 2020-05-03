#include "PlayerBall.h"

PlayerBall::PlayerBall(Mesh* mesh, Material* material, bool isFloor, DirectX::XMFLOAT4 playerPos, float mouseLookSpeed)
	: GameEntity(mesh, material, isFloor, playerPos)
{
	this->mouseLookSpeed = mouseLookSpeed;
	transform.SetPosition(playerPos.x, playerPos.y, playerPos.z);
}

void PlayerBall::Update(float dt, HWND windowHandle)
{
	// speed, 2 is hardcoded in
	float speed = dt * 8;

	// Basic movement
	if (GetAsyncKeyState('W') & 0x8000) { transform.MoveRelative(0, 0, speed); }
	
	//if (GetAsyncKeyState('S') & 0x8000) { transform.MoveRelative(0, 0, (speed) * -1); }
	//if (GetAsyncKeyState('D') & 0x8000) { transform.MoveRelative(speed, 0, 0); }
	//if (GetAsyncKeyState('A') & 0x8000) { transform.MoveRelative((speed) * -1, 0, 0); }
	//if (GetAsyncKeyState(VK_SPACE) & 0x8000) { transform.MoveRelative(0, speed, 0); }
	//if (GetAsyncKeyState('X') & 0x8000) { transform.MoveRelative(0, (speed) * -1, 0); }

	// Mouse look...

	POINT mousePos = {};
	GetCursorPos(&mousePos);
	ScreenToClient(windowHandle, &mousePos);
	float rotSpeed = dt * mouseLookSpeed;
	//rotate while lmb is down
	if (GetAsyncKeyState(VK_LBUTTON) & 0x8000)
	{
		float x = (mousePos.x - prevMousePosition.x) * rotSpeed;
		float y = (mousePos.y - prevMousePosition.y) * rotSpeed;

		transform.Rotate(y, x, 0);
	}
	prevMousePosition = mousePos;
	/**/
}

Transform* PlayerBall::GetTransform() { return &transform; }