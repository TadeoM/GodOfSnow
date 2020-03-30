#pragma once
#include <DirectXMath.h>
class Camera
{
public:
	Camera();
	~Camera();
	void Update(float deltaTime);
	DirectX::XMFLOAT3 GetPosition();
	DirectX::XMFLOAT3 GetRotation();
	float GetXRotation();
	float GetYRotation();
	void RotateX(float x);
	void RotateY(float y);
	DirectX::XMFLOAT4X4 GetViewMatrix();
	int count;
	void UpdateProjectionMatrix(float fov, float aspectRatio,float nearClip, float farClip);
	DirectX::XMFLOAT4X4 GetProjectionMatrix();

protected:
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 rotation;
	DirectX::XMFLOAT3 forward;
	DirectX::XMFLOAT3 right;
	DirectX::XMFLOAT3 up;
	DirectX::XMFLOAT3 UnitZ;
	DirectX::XMFLOAT3 worldUp;
	float XRotation;
	float YRotation;
	float dt;
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projectionMatrix;
};

