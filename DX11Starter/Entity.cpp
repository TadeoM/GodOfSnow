#include "Entity.h"
#include <DirectXMath.h>
#include "DXCore.h"
#include "Material.h"
using namespace DirectX;

Entity::Entity(Material* mat,Mesh* m,float xpos,float ypos,float zpos, float xrot, float yrot, float zrot, float xscale, float yscale, float zscale)
{
	XMMATRIX w = XMMatrixIdentity();
	XMStoreFloat4x4(&world, XMMatrixTranspose(w));
	mesh = m;
	position.x = xpos;
	position.y = ypos;
	position.z = zpos;
	rotation.x = xrot;
	rotation.y = yrot;
	rotation.z = zrot;
	scale.x = xscale;
	scale.y = yscale;
	scale.z = zscale;
	material = mat;

}

Entity::Entity(Material* mat,Mesh* m, XMFLOAT3 pos, XMFLOAT3 rot, XMFLOAT3 scale1)
{
	XMMATRIX w = XMMatrixIdentity();
	XMStoreFloat4x4(&world, XMMatrixTranspose(w));
	mesh = m;
	position = pos;
	rotation = rot;
	scale = scale1;
	material = mat;
}

Entity::Entity(Material* mat, Mesh* m)
{
	XMMATRIX w = XMMatrixIdentity();
	XMStoreFloat4x4(&world, XMMatrixTranspose(w));
	mesh = m;

	position.x = 0;
	position.y = 0;
	position.z = 0;
	rotation.x = 0;
	rotation.y = 0;
	rotation.z = 0;
	scale.x = 1;
	scale.y = 1;
	scale.z = 1;
	material = mat;

}


Entity::~Entity()
{
}


///Set the objects position
void Entity::SetPosition(XMFLOAT3 pos)
{
	position = pos;
}
void Entity::SetPositionX(float x)
{
	position.x = x;
}
void Entity::SetPositionY(float y)
{
	position.y = y;
}
void Entity::SetPositionZ(float z)
{
	position.z = z;
}

///Set the objects rotation
void Entity::SetRotation(XMFLOAT3 rot)
{
	rotation = rot;
}
void Entity::SetRotationX(float x)
{
	rotation.x = x;
}
void Entity::SetRotationY(float y)
{
	rotation.y = y;
}
void Entity::SetRotationZ(float z)
{
	rotation.z = z;
}

///Set the objects scale
void Entity::SetScale(XMFLOAT3 scale1)
{
	scale = scale1;
}

void Entity::SetScaleX(float x)
{
	scale.x = x;
}
void Entity::SetScaleY(float y)
{
	scale.y = y;
}
void Entity::SetScaleZ(float z)
{
	scale.z = z;
}

///Returns the current scale
DirectX::XMFLOAT3 Entity::GetScale()
{
	return scale;
}
///Returns the current rotation
DirectX::XMFLOAT3 Entity::GetRotation()
{
	return rotation;
}
///Returns the current position
DirectX::XMFLOAT3 Entity::GetPosition()
{
	return position;
}

///Move the object from the current position
void Entity::Move(DirectX::XMFLOAT3 movement)
{
	position.x += movement.x;
	position.y += movement.y;
	position.z += movement.z;
}

///Move the object from the current x position
void Entity::MoveX(float x)
{
	position.x += x;
}

///Move the object from the current y position
void Entity::MoveY(float y)
{
	position.y += y;
}

///Move the object from the current z position
void Entity::MoveZ(float z)
{
	position.z += z;
}

///Rotate the object
void Entity::Rotate(DirectX::XMFLOAT3 rotationAmount)
{
	rotation.x += rotationAmount.x;
	rotation.y += rotationAmount.y;
	rotation.z += rotationAmount.z;

}

///Rotate the object using the X axis
void Entity::RotateX(float x)
{
	rotation.x += x;
}

///Rotate the object using the y axis
void Entity::RotateY(float y)
{
	rotation.y += y;
}

///Rotate the object using the z axis
void Entity::RotateZ(float z)
{
	rotation.z += z;
}

///Scale the object
void Entity::Scale(DirectX::XMFLOAT3 scaleAmount)
{
	scale.x += scaleAmount.x;
	scale.y += scaleAmount.y;
	scale.z += scaleAmount.z;
}

///Scale the object in the X
void Entity::ScaleX(float x)
{
	scale.x += x;
}

///Scale the object in the y
void Entity::ScaleY(float y)
{
	scale.y += y;
}

///Scale the object in the Z
void Entity::ScaleZ(float z)
{
	scale.z += z;
}

///Update the World Matrix and return it
DirectX::XMFLOAT4X4 Entity::UpdateWorld()
{
	XMMATRIX pos = XMMatrixTranslation(position.x, position.y, position.z);
	XMMATRIX sc = XMMatrixScaling(scale.x,scale.y,scale.z);
	XMMATRIX rotX = XMMatrixRotationX(rotation.x);
	XMMATRIX rotY = XMMatrixRotationY(rotation.y);
	XMMATRIX rotZ = XMMatrixRotationZ(rotation.z);



	XMMATRIX w = sc * rotX * rotY * rotZ * pos;

	XMStoreFloat4x4(&world, XMMatrixTranspose(w));

	return world;

}

void Entity::PrepareMaterials(DirectX::XMFLOAT4X4 view, DirectX::XMFLOAT4X4 proj)
{

	// Send data to shader variables
	//  - Do this ONCE PER OBJECT you're drawing
	//  - This is actually a complex process of copying data to a local buffer
	//    and then copying that entire buffer to the GPU.  
	//  - The "SimpleShader" class handles all of that for you.
	material->GetVertexShader()->SetMatrix4x4("world", UpdateWorld());
	material->GetVertexShader()->SetMatrix4x4("view",view);
	material->GetVertexShader()->SetMatrix4x4("projection",proj);
	material->GetVertexShader()->CopyAllBufferData();


	// Set the vertex and pixel shaders to use for the next Draw() command
//  - These don't technically need to be set every frame...YET
//  - Once you start applying different shaders to different objects,
//    you'll need to swap the current shaders before each draw
	material->GetVertexShader()->SetShader();
	material->GetPixelShader()->SetShader();
}



