#include "Camera.h"
#include "Windows.h"


using namespace DirectX;



Camera::Camera()
{
	position.x = 0.0f;
	position.y = 0.0f;
	position.z = -5.0f;
	rotation = { 0.0f, 0.0f, 1.0f };
	rotation.x = 0.0f;
	rotation.y = 0.0f;
	rotation.z = 1.0f;
	UnitZ.x = 0.0f;
	UnitZ.y = 0.0f;
	UnitZ.z = 1.0f;
	worldUp.x = 0.0f;
	worldUp.y = 1.0f;
	worldUp.z = 0.0f;
	XRotation = 0.0f; //Initialize the X and Y rotation
	YRotation = 0.0f;


	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		1280.0f / 720.0f,		// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!

}


Camera::~Camera()
{
}

void Camera::Update(float deltaTime)
{
	dt = deltaTime;
	XMMATRIX XRotationMatrix = XMMatrixRotationX(GetXRotation()); //Find the Current X rotation
	XMMATRIX YRotationMatrix = XMMatrixRotationY(GetYRotation()); //Find the Current Y rotation
	XMMATRIX RotationMatrix = XRotationMatrix * YRotationMatrix; //Combine X and Y rotation
	XMVECTOR UnitZ1 = XMLoadFloat3(&UnitZ); //Default forward Vector
	XMVECTOR forward = XMVector3Transform(UnitZ1, RotationMatrix);
	XMVECTOR worldup = XMLoadFloat3(&worldUp);
	XMVECTOR right = XMVector3Cross(worldup, forward);//Cross the Global Up and the current rotation(Forward)
	XMVECTOR up = XMVector3Cross(right,forward); 
	up = -up;
	XMVECTOR currentPos = XMLoadFloat3(&position);
	XMStoreFloat4x4(&view, XMMatrixTranspose(XMMatrixLookToLH(currentPos, forward, up)));

	


	if (GetAsyncKeyState('W') & 0x8000) 
	{
		XMStoreFloat3(&position, XMLoadFloat3(&position) + forward * deltaTime* 2);
		
		
	}
	if (GetAsyncKeyState('A') & 0x8000) 
	{ 
		XMStoreFloat3(&position, XMLoadFloat3(&position) - right * deltaTime *2 );
	}
	if (GetAsyncKeyState('S') & 0x8000)
	{ 
		XMStoreFloat3(&position, XMLoadFloat3(&position) - forward * deltaTime* 2);
	}
	if (GetAsyncKeyState('D') & 0x8000)
	{
		XMStoreFloat3(&position, XMLoadFloat3(&position) + right * deltaTime*2);
	}
	if (GetAsyncKeyState('X') & 0x8000)
	{
		XMStoreFloat3(&position, XMLoadFloat3(&position) - XMLoadFloat3(&worldUp) * deltaTime*2);
	}
	if (GetAsyncKeyState(VK_SPACE) & 0x8000)
	{
		XMStoreFloat3(&position, XMLoadFloat3(&position) + XMLoadFloat3(&worldUp) * deltaTime*2);
	}
}

DirectX::XMFLOAT3 Camera::GetPosition()
{
	return position;
}

DirectX::XMFLOAT3 Camera::GetRotation()
{
	return rotation;
}

float Camera::GetXRotation()
{
	return XRotation;
}

float Camera::GetYRotation()
{
	return YRotation;
}

void Camera::RotateX(float x)
{
	XRotation += x * 2 * dt;


}
void Camera::RotateY(float y)
{
	YRotation += y* 2 * dt;
}

DirectX::XMFLOAT4X4 Camera::GetViewMatrix()
{
	return view;
}

void Camera::UpdateProjectionMatrix(float fov, float aspectRatio, float nearClip, float farClip)
{
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		fov,		// Field of View Angle
		aspectRatio,		// Aspect ratio
		nearClip,						// Near clip plane distance
		farClip);					// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}

DirectX::XMFLOAT4X4 Camera::GetProjectionMatrix()
{
	return projectionMatrix;
}

