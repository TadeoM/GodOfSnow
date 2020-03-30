#pragma once
#include "DXCore.h"
#include "SimpleShader.h"
#include "Vertex.h"
#include <DirectXMath.h>
#include <d3d11.h>



class Mesh
{
public:
	Mesh(Vertex * vertices, int numberOfVerticies, unsigned int * indicies, int numberOfIndicies, ID3D11Device * device);
	Mesh(const char* objFile, ID3D11Device* device);
	void CreateBuffers(Vertex * vertices, int numberOfVerticies, unsigned int * indicies, int numberOfIndicies, ID3D11Device * device);
	~Mesh();
	ID3D11Buffer * GetVertexBuffer();
	ID3D11Buffer * GetIndexBuffer();
	int GetIndexCount();
private:
	ID3D11Buffer* vertexBuffer; //Used to hold the verticies
	ID3D11Buffer* indexBuffer; //used to hold what verticies connect
	int  indicesInIndexBuffer;

};

