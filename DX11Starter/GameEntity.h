#pragma once

// Include libraries.
// #include "DXCore.h"
// #include "SimpleShader.h"
// #include <vector>
#include <memory>
#include "Mesh.h"
#include <DirectXMath.h>

class GameEntity
{
public:
	GameEntity(std::shared_ptr<Mesh> const& sharedMesh);	// Pointer to shared mesh.
	~GameEntity();

	// Accessors
	std::shared_ptr<Mesh> GetMeshReference() const;
	void LoadWorldMatrix(DirectX::XMFLOAT4X4& wmDestination) const; // Get the world matrix.
	void LoadPosition(DirectX::XMFLOAT3& posDestination) const; // Get entity position.
	void LoadScale(DirectX::XMFLOAT3& scaleDestination) const; // Get entity scale.
	void LoadRotation(DirectX::XMFLOAT4& rotDestination) const; // Return rotation quaternion.

	// Mutators
	void SetTranslation(float x, float y, float z);
	void SetScale(float x, float y, float z);
	void SetRotation(float pitchY, float yawX, float rollZ);

	// Service
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);
	void Move(float x, float y, float z);
	void MoveForward(float distance);

private:
	// Stores the shared mesh.
	std::shared_ptr<Mesh> sharedMesh;

	// Sets the default transformation values.
	void CreateTransformations();

	// Transformations
	DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT3 position;
	DirectX::XMFLOAT3 scale;
	DirectX::XMFLOAT4 rotation;
};

