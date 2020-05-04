#include "Game.h"
#include "Vertex.h"

#include "WICTextureLoader.h"

// Needed for a helper function to read compiled shader files from the hard drive
#pragma comment(lib, "d3dcompiler.lib")
#include <d3dcompiler.h>

//mem leak stuff
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

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
		hInstance,		   // The application's handle
		"DirectX Game",	   // Text for the window's title bar
		1280,			   // Width of the window's client area
		720,			   // Height of the window's client area
		true)			   // Show extra stats (fps) in title bar?
{

	camera = 0;

#if defined(DEBUG) || defined(_DEBUG)
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.\n");
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
#endif

}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{
	printf("zoinks");
	// Since we've created the Mesh objects within this class (Game),
	// this is also where we should delete them!
	for (auto& m : meshes)
	{
		// Delete the current Mesh* from the meshes vector
		delete m;
	}
	
	// We've also created the entities here, so here is
	// where we delete them
	for (auto& e : entities)
	{
		delete e;
	}

	for (auto& f : materials)
	{
		delete f;
	}

	delete particleVS;
	delete particlePS;
	delete emitter;

	delete camera;

	delete vertexShader;
	delete pixelShader;

	delete vertexShaderNM;
	delete pixelShaderNM;

	_CrtDumpMemoryLeaks();
	printf("heck");
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
	CreateBasicGeometry();

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.  
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Make our camera
	camera = new Camera(0, -30, -50, this->width / (float)this->height, 1.0f);
	//camera->GetTransform()->Rotate(0.4, 0, 0);

	//stuff for particles ----------------------
	ParticleSetup();
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files
// and also created the Input Layout that describes our 
// vertex data to the rendering pipeline. 
// - Input Layout creation is done here because it must 
//    be verified against vertex shader byte code
// - We'll have that byte code already loaded below
// --------------------------------------------------------
void Game::LoadShaders()
{
	vertexShader = new SimpleVertexShader(
		device.Get(),
		context.Get(),
		GetFullPathTo_Wide(L"VertexShader.cso").c_str());
	pixelShader = new SimplePixelShader(
		device.Get(),
		context.Get(),
		GetFullPathTo_Wide(L"PixelShader.cso").c_str());
	vertexShaderNM = new SimpleVertexShader(
		device.Get(),
		context.Get(),
		GetFullPathTo_Wide(L"NormalMapVS.cso").c_str());
	pixelShaderNM = new SimplePixelShader(
		device.Get(),
		context.Get(),
		GetFullPathTo_Wide(L"NormalMapPS.cso").c_str());

	particleVS = new SimpleVertexShader(
		device.Get(),
		context.Get(),
		GetFullPathTo_Wide(L"ParticleVS.cso").c_str());

	particlePS = new SimplePixelShader(
		device.Get(),
		context.Get(),
		GetFullPathTo_Wide(L"ParticlePS.cso").c_str());

}

void Game::ParticleSetup()
{
	// Particle setup ====================
	CreateWICTextureFromFile(device.Get(), context.Get(), GetFullPathTo_Wide(L"../../Assets/Textures/particle.jpg").c_str(), 0, particleTexture.GetAddressOf());

	// A depth state for the particles
	D3D11_DEPTH_STENCIL_DESC dsDesc = {};
	dsDesc.DepthEnable = true;
	dsDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ZERO; // Turns off depth writing
	dsDesc.DepthFunc = D3D11_COMPARISON_LESS;
	device->CreateDepthStencilState(&dsDesc, particleDepthState.GetAddressOf());


	// Blend for particles (additive)
	D3D11_BLEND_DESC blend = {};
	blend.AlphaToCoverageEnable = false;
	blend.IndependentBlendEnable = false;
	blend.RenderTarget[0].BlendEnable = true;
	blend.RenderTarget[0].BlendOp = D3D11_BLEND_OP_ADD;
	blend.RenderTarget[0].SrcBlend = D3D11_BLEND_SRC_ALPHA; // Still respect pixel shader output alpha
	blend.RenderTarget[0].DestBlend = D3D11_BLEND_ONE;
	blend.RenderTarget[0].BlendOpAlpha = D3D11_BLEND_OP_ADD;
	blend.RenderTarget[0].SrcBlendAlpha = D3D11_BLEND_ONE;
	blend.RenderTarget[0].DestBlendAlpha = D3D11_BLEND_ONE;
	blend.RenderTarget[0].RenderTargetWriteMask = D3D11_COLOR_WRITE_ENABLE_ALL;
	device->CreateBlendState(&blend, particleBlendState.GetAddressOf());

	// Debug rasterizer state for particles
	D3D11_RASTERIZER_DESC rd = {};
	rd.CullMode = D3D11_CULL_BACK;
	rd.DepthClipEnable = true;
	rd.FillMode = D3D11_FILL_WIREFRAME;
	device->CreateRasterizerState(&rd, particleDebugRasterState.GetAddressOf());

	// Set up particles
	emitter = new Emitter(
		30000,							// Max particles
		100,							// Particles per second
		20,								// Particle lifetime
		0.04f,							// Start size
		0.04f,							// End size
		XMFLOAT4(1, 1.0f, 1.0f, 1.0f),	// Start color
		XMFLOAT4(1, 1.0f, 1.0f, 0),		// End color
		XMFLOAT3(0, -0.5f, 0),				// Start velocity
		XMFLOAT3(0.2f, 0.2f, 0.2f),		// Velocity randomness range
		XMFLOAT3(0, -36, 0),				// Emitter position5
		XMFLOAT3(20.0f, 8.0f, 20.0f),		// Position randomness range
		XMFLOAT4(-2, 2, -2, 2),			// Random rotation ranges (startMin, startMax, endMin, endMax)
		XMFLOAT3(0, -0.4f, 0),				// Constant acceleration
		device,
		particleVS,
		particlePS,
		particleTexture);
}

// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{
	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable
	XMFLOAT4 red = XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f);
	XMFLOAT4 green = XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f);
	XMFLOAT4 blue = XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f);
	XMFLOAT4 white = XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f);

	// new meshes
	Mesh* mesh1 = new Mesh(GetFullPathTo("../../Assets/Models/sphere.obj").c_str(), device);
	Mesh* mesh2 = new Mesh(GetFullPathTo("../../Assets/Models/helix.obj").c_str(), device);
	Mesh* mesh3 = new Mesh(GetFullPathTo("../../Assets/Models/torus.obj").c_str(), device);
	Mesh* mesh4 = new Mesh(GetFullPathTo("../../Assets/Models/plane.obj").c_str(), device);

	meshes.push_back(mesh1);
	meshes.push_back(mesh2);
	meshes.push_back(mesh3);
	meshes.push_back(mesh4);

	CreateWICTextureFromFile(
		device.Get(),
		context.Get(),
		GetFullPathTo_Wide(L"../../Assets/Textures/indent.png").c_str(),
		nullptr,
		indentTexture.GetAddressOf());
	CreateWICTextureFromFile(
		device.Get(),
		context.Get(),
		GetFullPathTo_Wide(L"../../Assets/Textures/paving.png").c_str(),
		nullptr,
		diffuseTexture2.GetAddressOf());
	CreateWICTextureFromFile(
		device.Get(),
		context.Get(),
		GetFullPathTo_Wide(L"../../Assets/Textures/rock.png").c_str(),
		nullptr,
		rockDiffuse.GetAddressOf());
	CreateWICTextureFromFile(
		device.Get(),
		context.Get(),
		GetFullPathTo_Wide(L"../../Assets/Textures/rock_normals.png").c_str(),
		nullptr,
		rockNormalMap.GetAddressOf());

	//load snow textures
	CreateWICTextureFromFile(
		device.Get(),
		context.Get(),
		GetFullPathTo_Wide(L"../../Assets/Textures/snow_display.png").c_str(),
		nullptr,
		snowDiffuse.GetAddressOf());
	CreateWICTextureFromFile(
		device.Get(),
		context.Get(),
		GetFullPathTo_Wide(L"../../Assets/Textures/snow_normals.jpg").c_str(),
		nullptr,
		snowNormal.GetAddressOf());

	D3D11_SAMPLER_DESC sampDesc = {};
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_CLAMP;
	sampDesc.Filter = D3D11_FILTER_ANISOTROPIC;
	sampDesc.MaxAnisotropy = 16;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;
	device->CreateSamplerState(&sampDesc, samplerOptions.GetAddressOf());

	//create the materials
	Material* mat1 = new Material(pixelShader, vertexShader, white, 64, indentTexture, samplerOptions, nullptr, nullptr);
	Material* mat2 = new Material(pixelShaderNM, vertexShaderNM, white, 4, rockDiffuse, samplerOptions, rockNormalMap, indentTexture);
	Material* mat3 = new Material(pixelShader, vertexShader, white, 256, diffuseTexture2, samplerOptions, nullptr, nullptr);
	Material* snowMat = new Material(pixelShaderNM, vertexShaderNM, white, 4, snowDiffuse, samplerOptions, snowNormal, indentTexture);

	mat1Shiny = mat1->GetShiny();
	mat2Shiny = mat2->GetShiny();
	mat3Shiny = mat3->GetShiny();
	materials.push_back(mat1);
	materials.push_back(mat2);
	materials.push_back(mat3);
	materials.push_back(snowMat);

	decalPosition = XMFLOAT3(3, 3, 3);
	// Create the game entities
	GameEntity* g1 = new GameEntity(mesh4, snowMat, true, decalPosition);
	GameEntity* g2 = new GameEntity(mesh1, mat2, false, decalPosition);

	// Add to GameEntity vector (easier to loop through and clean up)
	entities.push_back(g1);
	entities.push_back(g2);

	entities[0]->GetTransform()->SetPosition(0, -50, 0);
	entities[1]->GetTransform()->SetScale(5, 5, 5);
	entities[1]->GetTransform()->SetPosition(0, -49, 0);
}


// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();

	if (camera)
		camera->UpdateProjectionMatrix(this->width / (float)this->height);
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();

	//entities[0]->GetTransform()->MoveAbsolute(.2*deltaTime, 0, 0);
	float playerSpeed = deltaTime * 10;
	//entities[1]->GetTransform()->MoveAbsolute(0, 0, playerSpeed);
	DirectX::XMFLOAT3 pos = entities[1]->GetTransform()->GetPosition();
	if (GetAsyncKeyState('W') && pos.z < 22) 
	{ entities[1]->GetTransform()->MoveAbsolute(0, 0, playerSpeed); }
	if (GetAsyncKeyState('S') && pos.z > -22)
	{ entities[1]->GetTransform()->MoveAbsolute(0, 0, (playerSpeed) * -1); }
	if (GetAsyncKeyState('D') && pos.x < 22)
	{ entities[1]->GetTransform()->MoveAbsolute(playerSpeed, 0, 0); }
	if (GetAsyncKeyState('A') && pos.x > -22)
	{ entities[1]->GetTransform()->MoveAbsolute((playerSpeed) * -1, 0, 0); }
	//printf("%f %f %f\n", entities[1]->GetTransform()->GetPosition().x, entities[1]->GetTransform()->GetPosition().y, entities[1]->GetTransform()->GetPosition().z);
	//entities[1]->GetTransform()->MoveAbsolute(.2*deltaTime, .2*deltaTime, 0);
	/*entities[2]->GetTransform()->MoveAbsolute(-0.2*deltaTime, -0.2*deltaTime, 0);
	entities[3]->GetTransform()->SetRotation(0, 0, entities[3]->GetTransform()->GetPitchYawRoll().z+.5*deltaTime);
	entities[4]->GetTransform()->SetRotation(0, 0, entities[4]->GetTransform()->GetPitchYawRoll().z + -.5 * deltaTime);*/

	entities[0]->UpdatePlayPosHistory(entities[1]->GetTransform()->GetPosition());

	camera->Update(deltaTime, this->hWnd);

	emitter->Update(deltaTime);
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Background color (black to see lighting) for clearing
	const float color[4] = { 0.0f, 0.0f, 0.0f, 0.0f };

	context->ClearRenderTargetView(backBufferRTV.Get(), color);
	context->ClearDepthStencilView(
		depthStencilView.Get(),
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);


	// fill light, moved here from init from last time
	dLight1.AmbientColor = DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f);
	dLight1.DiffuseColor = DirectX::XMFLOAT3(1,1, 1);
	dLight1.Direction = DirectX::XMFLOAT3(1, -1, 0);

	dLight2.AmbientColor = DirectX::XMFLOAT3(1, 1, 1);
	dLight2.Direction = DirectX::XMFLOAT3(1, 1, 0);

	dLight3.AmbientColor = DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f);
	dLight3.DiffuseColor = DirectX::XMFLOAT3(1,1,1);
	dLight3.Direction = DirectX::XMFLOAT3(0, -1, 1);

	pLight1.AmbientColor = DirectX::XMFLOAT3(0.1f, 0.1f, 0.1f);
	pLight1.DiffuseColor = DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f);
	pLight1.Position = DirectX::XMFLOAT3(0, 2, 0);

	pixelShader->SetData("dLight1", &dLight1, sizeof(DirectionalLight));
	pixelShader->SetData("dLight2", &dLight2, sizeof(DirectionalLight));
	pixelShader->SetData("dLight3", &dLight3, sizeof(DirectionalLight));
	pixelShader->SetData("pLight1", &pLight1, sizeof(PointLight));
	pixelShader->SetFloat3("cameraPosition", camera->GetTransform()->GetPosition());
	pixelShader->SetFloat("material1Shiny", mat1Shiny);
	pixelShader->SetFloat("material2Shiny", mat2Shiny);
	pixelShader->SetFloat("material3Shiny", mat3Shiny);
	pixelShader->CopyAllBufferData();

	pixelShaderNM->SetData("dLight1", &dLight1, sizeof(DirectionalLight));
	pixelShaderNM->SetData("dLight2", &dLight2, sizeof(DirectionalLight));
	pixelShaderNM->SetData("dLight3", &dLight3, sizeof(DirectionalLight));
	pixelShaderNM->SetData("pLight1", &pLight1, sizeof(PointLight));
	pixelShaderNM->SetFloat3("cameraPosition", camera->GetTransform()->GetPosition());
	pixelShaderNM->SetFloat("material1Shiny", mat1Shiny);
	pixelShaderNM->SetFloat("material2Shiny", mat2Shiny);
	pixelShaderNM->SetFloat("material3Shiny", mat3Shiny);
	pixelShaderNM->CopyAllBufferData();

	// Loop through the game entities and draw
	for (auto& e : entities)
	{
		// Set texture resources for the next draw
		e->GetMaterial()->GetVertexShader()->SetShader();
		e->GetMaterial()->GetPixelShader()->SetShader();
		if (e->GetMaterial()->GetNormalMap() != nullptr) {
			pixelShaderNM->SetShaderResourceView("normalMap", e->GetMaterial()->GetNormalMap().Get());
			pixelShaderNM->SetShaderResourceView("diffuseTexture", e->GetMaterial()->GetDiffuseTexture().Get());
			// pass indent texture to pixel shader AND vertex shader
			pixelShaderNM->SetShaderResourceView("indentTexture", e->GetMaterial()->GetIndentTexture().Get());
			vertexShaderNM->SetShaderResourceView("indentTexture", e->GetMaterial()->GetIndentTexture().Get());
			pixelShaderNM->SetSamplerState("samplerOptions", samplerOptions.Get());
			vertexShaderNM->SetSamplerState("samplerOptions", samplerOptions.Get());
		}
		else {
			pixelShader->SetShaderResourceView("diffuseTexture", e->GetMaterial()->GetDiffuseTexture().Get());
			pixelShader->SetSamplerState("samplerOptions", samplerOptions.Get());
		}
		e->Draw(context, camera);
	}

	// Particle drawing =============
	{

		// Particle states
		context->OMSetBlendState(particleBlendState.Get(), 0, 0xffffffff);	// Additive blending
		context->OMSetDepthStencilState(particleDepthState.Get(), 0);		// No depth WRITING

		// No wireframe debug
		particlePS->SetInt("debugWireframe", 0);
		particlePS->CopyAllBufferData();

		// Draw the emitter
		emitter->Draw(context, camera);

		if (GetAsyncKeyState('C'))
		{
			context->RSSetState(particleDebugRasterState.Get());
			particlePS->SetInt("debugWireframe", 1);
			particlePS->CopyAllBufferData();

			emitter->Draw(context, camera);
		}

		// Reset to default states for next frame
		context->OMSetBlendState(0, 0, 0xffffffff);
		context->OMSetDepthStencilState(0, 0);
		context->RSSetState(0);
	}



	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);

	// Due to the usage of a more sophisticated swap chain,
	// the render target must be re-bound after every call to Present()
	context->OMSetRenderTargets(1, backBufferRTV.GetAddressOf(), depthStencilView.Get());
}