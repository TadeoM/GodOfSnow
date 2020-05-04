#include "Camera.h"
#include <Windows.h>

using namespace DirectX;

// Creates a camera at the specified position
Camera::Camera(float x, float y, float z, float aspectRatio, float mouseLookSpeed)
{
	// Save speed and pos
	this->mouseLookSpeed = mouseLookSpeed;
	transform.SetPosition(x, y, z);

	// Update our view & proj
	UpdateViewMatrix();
	UpdateProjectionMatrix(aspectRatio);
}

// Clean up if necessary
Camera::~Camera()
{ 
	
}


// Camera's update, which looks for key presses
void Camera::Update(float dt, HWND windowHandle)
{
	// speed, 2 is hardcoded in
	float speed = dt * 15;

	// Adjust speed if necessary based on keys...

	// Basic movement
	if (GetAsyncKeyState('W') & 0x8000) { transform.MoveRelative(0, 0, speed); }
	if (GetAsyncKeyState('S') & 0x8000) { transform.MoveRelative(0, 0, (speed) * -1); }
	if (GetAsyncKeyState('D') & 0x8000) { transform.MoveRelative(speed, 0, 0); }
	if (GetAsyncKeyState('A') & 0x8000) { transform.MoveRelative((speed) * -1, 0, 0); }
	if (GetAsyncKeyState(VK_SPACE) & 0x8000) { transform.MoveRelative(0, speed, 0); }
	if (GetAsyncKeyState('X') & 0x8000) { transform.MoveRelative(0, (speed) * -1, 0); }

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
	// Every frame, update the view matrix
	UpdateViewMatrix();
}

// Creates a new view matrix based on current position and orientation
void Camera::UpdateViewMatrix()
{
	// Figure out our "forward"  vector and
	// create a quaternion using OUR rotation
	XMFLOAT3 rotation = transform.GetPitchYawRoll();
	XMVECTOR rotQuat = XMQuaternionRotationRollPitchYawFromVector(XMLoadFloat3(&rotation));

	// Define the "standard" forward vector w/o rotation (0,0,1)
	XMVECTOR basicForward = XMVectorSet(0, 0, 1, 0);

	// Rotate the "standard forward" by OUR rotation
	XMVECTOR ourForward = XMVector3Rotate(basicForward, rotQuat);

	// Create the final view matrix
	XMMATRIX view = XMMatrixLookToLH(
		XMLoadFloat3(&transform.GetPosition()),
		ourForward,
		XMVectorSet(0, 1, 0, 0));
	XMStoreFloat4x4(&viewMatrix, view);
}

// Updates the projection matrix
void Camera::UpdateProjectionMatrix(float aspectRatio)
{
	XMMATRIX proj = XMMatrixPerspectiveFovLH(
		XM_PIDIV4,
		aspectRatio,
		0.01f,
		100.0f);
	XMStoreFloat4x4(&projMatrix, proj);
}

Transform* Camera::GetTransform()
{
	return &transform;
}
