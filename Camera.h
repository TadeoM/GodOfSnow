#pragma once
#include <DirectXMath.h>
#include <Windows.h>

#include "Transform.h"

class Camera
{
public:
	Camera(float x, float y, float z, float aspectRatio, float mouseLookSpeed);
	~Camera();

	// Updating
	void Update(float dt, HWND windowHandle);
	void UpdateViewMatrix();
	void UpdateProjectionMatrix(float aspectRatio);

	// Getters
	DirectX::XMFLOAT4X4 GetView() { return viewMatrix; }
	DirectX::XMFLOAT4X4 GetProjection() { return projMatrix; }

	Transform* GetTransform();

private:
	// Camera matrices
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projMatrix;

	float mouseLookSpeed;
	POINT prevMousePosition;
	Transform transform;

};

