#pragma once

#include "DXCore.h"
#include "Mesh.h"
#include "GameEntity.h"
#include "Camera.h"
#include "Material.h"
#include "SimpleShader.h"
#include "Lights.h"

#include <DirectXMath.h>
#include <wrl/client.h> // Used for ComPtr - a smart pointer for COM objects
#include <vector>

//this is just a test comment to see if my repo is set up
class Game
	: public DXCore
{

public:
	Game(HINSTANCE hInstance);
	~Game();

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);

private:

	// Initialization helper methods - feel free to customize, combine, etc.
	void LoadShaders();
	void CreateBasicGeometry();

	// Vectors for things we need to clean up
	std::vector<Mesh*> meshes;
	std::vector<GameEntity*> entities;
	std::vector<Material*> materials;

	// Our game camera
	Camera* camera;


	// Shaders and shader-related constructs
	SimplePixelShader* pixelShader;
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShaderNM;
	SimpleVertexShader* vertexShaderNM;

	// lights
	DirectionalLight dLight1;
	DirectionalLight dLight2;
	DirectionalLight dLight3;

	PointLight pLight1;

	float mat1Shiny;
	float mat2Shiny;
	float mat3Shiny;
	DirectX::XMFLOAT4 decalPosition;

	//snow texture related resources
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> snowDiffuse;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> snowNormal;

	// Texture related resources
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> indentTexture;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> diffuseTexture2;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> rockDiffuse;
	Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> rockNormalMap;
	Microsoft::WRL::ComPtr<ID3D11SamplerState> samplerOptions;
};

