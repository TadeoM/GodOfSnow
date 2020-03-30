#include "Game.h"
#include "Vertex.h"
#include "Entity.h"

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore(
		hInstance,		// The application's handle
		"DirectX Game",	   	// Text for the window's title bar
		1280,			// Width of the window's client area
		720,			// Height of the window's client area
		true)			// Show extra stats (fps) in title bar?
{
	// Initialize fields
	vertexShader = 0;
	pixelShader = 0;
	mouseDown = false;
	

#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.\n");
#endif
	
}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{

	// Delete our simple shader objects, which
	// will clean up their own internal DirectX stuff
	delete vertexShader;
	delete pixelShader;

	delete sphereMesh;
	delete sphere;
	delete helixMesh;
	delete helix;
	delete torusMesh;
	delete torus;

	
	delete material1;

	delete main;

}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	LoadShaders();
	CreateMatrices();
	CreateBasicGeometry();

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
	
	
	DirectionalLight.AmbientColor = { 0.1f,0.1f,0.1f,1.0f};
	DirectionalLight.DiffuseColor = { 0.0f,0.0f,1.0f,1.0f };
	DirectionalLight.Direction = { 1.0f,-1.0f,0.0f };

	DirectionalLight2.AmbientColor = { 0.1f,0.1f,0.1f,1.0f };
	DirectionalLight2.DiffuseColor = { 1.0f,0.647059f,0.0f,1.0f };
	DirectionalLight2.Direction = { -0.5f,0.75f,0.0f };



	pixelShader->SetData("light1", &DirectionalLight, sizeof(Light));
	pixelShader->SetData("light2", &DirectionalLight2, sizeof(Light));
	pixelShader->CopyAllBufferData(); // PUSH TO GPU HERE!

	
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files using
// my SimpleShader wrapper for DirectX shader manipulation.
// - SimpleShader provides helpful methods for sending
//   data to individual variables on the GPU
// --------------------------------------------------------
void Game::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, context);
	vertexShader->LoadShaderFile(L"VertexShader.cso");

	pixelShader = new SimplePixelShader(device, context);
	pixelShader->LoadShaderFile(L"PixelShader.cso");

	material1 = new Material(pixelShader, vertexShader);
}



// --------------------------------------------------------
// Initializes the matrices necessary to represent our geometry's 
// transformations and our 3D camera
// --------------------------------------------------------
void Game::CreateMatrices()
{
	// Create the View matrix
	// - In an actual game, recreate this matrix every time the camera 
	//    moves (potentially every frame)
	// - We're using the LOOK TO function, which takes the position of the
	//    camera and the direction vector along which to look (as well as "up")
	// - Another option is the LOOK AT function, to look towards a specific
	//    point in 3D space
	XMVECTOR pos = XMVectorSet(0, 0, -5, 0);
	XMVECTOR dir = XMVectorSet(0, 0, 1, 0);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	XMMATRIX V = XMMatrixLookToLH(
		pos,     // The position of the "camera"
		dir,     // Direction the camera is looking
		up);     // "Up" direction in 3D space (prevents roll)
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V)); // Transpose for HLSL!

	// Create the Projection matrix
	// - This should match the window's aspect ratio, and also update anytime
	//    the window resizes (which is already happening in OnResize() below)
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		(float)width / height,		// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!






}


// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{
	

	sphereMesh = new Mesh("Assets/Models/sphere.obj", device);
	helixMesh = new Mesh("Assets/Models/helix.obj", device);
	torusMesh = new Mesh("Assets/Models/torus.obj", device);


	// Make some entities
	sphere = new Entity(material1, sphereMesh);
	helix = new Entity(material1, helixMesh);
	torus = new Entity(material1,torusMesh);

	sphere->SetPositionX(3.0f);
	torus->SetPositionX(-3.0f);
	main = new Camera();
}


// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();

	main->UpdateProjectionMatrix(0.25f * 3.1415926535f,(float)width / height,0.1f,100.0f);
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();

	main->Update(deltaTime);

}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	// Send data to shader variables
	//  - Do this ONCE PER OBJECT you're drawing
	//  - This is actually a complex process of copying data to a local buffer
	//    and then copying that entire buffer to the GPU.  
	//  - The "SimpleShader" class handles all of that for you.
	sphere->PrepareMaterials(main->GetViewMatrix(), main->GetProjectionMatrix());

	
//	// Set buffers in the input assembler
//	//  - Do this ONCE PER OBJECT you're drawing, since each object might
//	//    have different geometry.
	UINT stride = sizeof(Vertex);
	UINT offset = 0;
	//Shape 1
	ID3D11Buffer* Shape1VB = sphereMesh->GetVertexBuffer();
	context->IASetVertexBuffers(0, 1, &Shape1VB, &stride, &offset);
	context->IASetIndexBuffer(sphereMesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
//	// Finally do the actual drawing
////  - Do this ONCE PER OBJECT you intend to draw
////  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
////  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
////     vertices in the currently set VERTEX BUFFER
	context->DrawIndexed(
		sphereMesh->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);    // Offset to add to each index when looking up vertices



	helix->PrepareMaterials(main->GetViewMatrix(), main->GetProjectionMatrix());

	//Draw Shape 2
	ID3D11Buffer* Shape2VB = helixMesh->GetVertexBuffer();
	context->IASetVertexBuffers(0, 1, &Shape2VB, &stride, &offset);
	context->IASetIndexBuffer(helixMesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
	context->DrawIndexed(
		helixMesh->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);    // Offset to add to each index when looking up vertices
	
	torus->PrepareMaterials(main->GetViewMatrix(), main->GetProjectionMatrix());

	//DrawShape 3
	ID3D11Buffer* Shape3VB = torusMesh->GetVertexBuffer();
	context->IASetVertexBuffers(0, 1, &Shape3VB, &stride, &offset);
	context->IASetIndexBuffer(torusMesh->GetIndexBuffer(), DXGI_FORMAT_R32_UINT, 0);
	context->DrawIndexed(
		torusMesh->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
		0,     // Offset to the first index we want to use
		0);    // Offset to add to each index when looking up vertices

	swapChain->Present(0, 0);
}


#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
// --------------------------------------------------------
void Game::OnMouseDown(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...
	mouseDown = true;

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;

	// Caputure the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
// --------------------------------------------------------
void Game::OnMouseUp(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...
	mouseDown = false;
	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're 
// currently capturing the mouse.
// --------------------------------------------------------
void Game::OnMouseMove(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...
	if (mouseDown)
	{
		main->RotateX(prevMousePos.y - y);
		main->RotateY(prevMousePos.x - x);
	}


	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;


}

// --------------------------------------------------------
// Helper method for mouse wheel scrolling.  
// WheelDelta may be positive or negative, depending 
// on the direction of the scroll
// --------------------------------------------------------
void Game::OnMouseWheel(float wheelDelta, int x, int y)
{
	// Add any custom code here...
}
#pragma endregion