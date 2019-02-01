#pragma once

// -----------------------------------------------
// Include statements.
// -----------------------------------------------
#include <memory>
#include <queue>
#include <vector>
#include <DirectXMath.h>
#include "Mesh.h"
#include "Transform.h"

class GameEntity
{
public:

	// -----------------------------------------------
	// Constructors.
	// -----------------------------------------------

	// Pointer to shared mesh.
	GameEntity(std::shared_ptr<Mesh>& sharedMesh);

	// Copy constructor.
	GameEntity(GameEntity& other);

	// Initialize game entity with a starting position.
	GameEntity(std::shared_ptr<Mesh>& sharedMesh, float pX, float pY, float pZ);

	// Initialize game entity with a starting position.
	GameEntity(std::shared_ptr<Mesh>& sharedMesh,
		float pX, float pY, float pZ,
		float sX, float sY, float sZ,
		float rX, float rY, float rZ, float rW);

	// Initialize game entity with existing Transform.
	GameEntity(std::shared_ptr<Mesh>& sharedMesh, TRANSFORM transformation);

	// Destructor.
	~GameEntity();

	// -----------------------------------------------
	// Static methods.
	// -----------------------------------------------

	// ----------
	// FACTORY METHODS

	// Factory method to create a single entity.
	static void CreateGameEntity(GameEntity& gameEntity, std::shared_ptr<Mesh>& sharedMesh);

	// Create specified number of game entities and append them to an existing std::vector.
	static void CreateGameEntities(std::vector<std::unique_ptr<GameEntity>>& gameEntities, std::shared_ptr<Mesh>& sharedMesh, int count = 1);

	// -----------------------------------------------
	// Mutators.
	// -----------------------------------------------

	// ----------
	// TRANSFORM
	void SetTransform(const TRANSFORM& transformation);	// Set the value of the internal transform object to the parameter.

	// ----------
	// POSITION
	void SetPosition(float x, float y, float z);
	void SetPosition(_In_reads_(3) const float *sourceArray);
	void SetPosition(const DirectX::XMFLOAT3& source);
	void SetPosition(const TRANSFORM& source);

	// ----------
	// SCALE
	void SetScale(float x, float y, float z);
	void SetScale(_In_reads_(3) const float *sourceArray);
	void SetScale(const DirectX::XMFLOAT3& source);
	void SetScale(const TRANSFORM& source);

	// ----------
	// ROTATION
	void SetRotation(float x, float y, float z, float w);
	void SetRotation(_In_reads_(4) const float *sourceArray);
	void SetRotation(const DirectX::XMFLOAT4& source);
	void SetRotation(const TRANSFORM& source);

	// -----------------------------------------------
	// Accessors.
	// -----------------------------------------------

	// ----------
	// TRANSFORM
	const TRANSFORM GetTransform() const; // Creates a const containing a copy of the Entity's transform values.
	void LoadTransform(TRANSFORM& target) const;

	// ----------
	// POSITION
	const DirectX::XMFLOAT3 GetPosition() const;
	void LoadPosition(DirectX::XMFLOAT3& target) const;

	// ----------
	// SCALE
	const DirectX::XMFLOAT3 GetScale() const;
	void LoadScale(DirectX::XMFLOAT3& target) const;

	// ----------
	// ROTATION
	const DirectX::XMFLOAT4 GetRotation() const;
	void LoadRotation(DirectX::XMFLOAT4& target) const;

	// ----------
	// WORLD MATRIX
	const DirectX::XMFLOAT4X4 GetWorldMatrix() const;
	void LoadWorldMatrix(DirectX::XMFLOAT4X4& target) const;

	// ----------
	// MESH
	const std::shared_ptr<Mesh> GetMesh() const;

	// -----------------------------------------------
	// Service methods.
	// -----------------------------------------------

	// ----------
	// UPDATE
	void Update(float deltaTime, float totalTime);

	// ----------
	// ROTATION
	void SetRotationRollPitchYaw(float pitchY, float yawX, float rollZ);
	void SetRotationRollPitchYaw(_In_reads_(3) const float *sourceArray);

	// ----------
	// WORLD MATRIX
	void CalculateWorldMatrix(DirectX::XMFLOAT4X4& target) const;


	// Service
	void Update(float deltaTime, float totalTime);
	void UpdatePosition(float deltaTime, float totalTime, DirectX::XMFLOAT3 moveSpeed);
	void UpdateRotation(float deltaTime, float totalTime, DirectX::XMFLOAT3 rotationSpeed);
	void UpdateScale(float deltaTime, float totalTime, DirectX::XMFLOAT3 scaleSpeed);
	void Move(float x, float y, float z);
	// void MoveForward(float distance);

private:

	// -----------------------------------------------
	// Data members.
	// -----------------------------------------------

	// Stores the shared mesh.
	std::shared_ptr<Mesh> sharedMesh;

	// Enqueued transformation changes.

	// Transformation storage.
	TRANSFORM localTransform;

	// -----------------------------------------------
	// Helper methods.
	// -----------------------------------------------

	// En

	// Sets the default transformation values.
	void CreateTransformations();

};

