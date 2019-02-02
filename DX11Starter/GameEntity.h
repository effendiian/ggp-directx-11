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
#include "TransformBuffer.h"

class GameEntity
{
public:
	// -----------------------------------------------
	// Internal typedef/enum statements.
	// -----------------------------------------------

	/// <summary>
	/// Shared pointer reference to a mesh instance.
	/// </summary>
	typedef std::shared_ptr<Mesh> MeshReference;

	/// <summary>
	/// Unique pointer to a given GameEntity.
	/// </summary>
	typedef std::unique_ptr<GameEntity> GameEntityReference;

	/// <summary>
	/// Collection of unique GameEntity pointers.
	/// </summary>
	typedef std::vector<GameEntityReference> GameEntityCollection;


	// -----------------------------------------------
	// Constructors.
	// -----------------------------------------------

	// Pointer to shared mesh.
	GameEntity(MeshReference& sharedMesh);

	// Copy constructor.
	GameEntity(GameEntity& other);

	// Copy assignment.
	GameEntity& operator=(GameEntity other);

	// Initialize game entity with a starting position.
	GameEntity(MeshReference& sharedMesh, float pX, float pY, float pZ);

	// Initialize game entity with a starting position and scale.
	GameEntity(MeshReference& sharedMesh,
		float pX, float pY, float pZ,
		float sX, float sY, float sZ);

	// Initialize game entity with a starting position, scale, and rotation.
	GameEntity(MeshReference& sharedMesh,
		float pX, float pY, float pZ,
		float sX, float sY, float sZ,
		float rX, float rY, float rZ, float rW);

	// Initialize game entity with existing Transform.
	GameEntity(MeshReference& sharedMesh, const TRANSFORM t);

	// Destructor.
	~GameEntity();

	// -----------------------------------------------
	// Static methods.
	// -----------------------------------------------

	// ----------
	// FACTORY METHODS

	// Factory method to create a single entity.
	static void CreateGameEntity(GameEntity& gameEntity, MeshReference& sharedMesh);

	// Create specified number of game entities and append them to an existing std::vector.
	static void CreateGameEntities(GameEntityCollection& gameEntities, MeshReference& sharedMesh, int count = 1);

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
	const MeshReference& GetMesh() const;

	// -----------------------------------------------
	// Mutators.
	// -----------------------------------------------

	// ----------
	// TRANSFORM
	void SetTransform(const TRANSFORM& transformation);	// Set the value of the internal transform object to the parameter.

	// -----------------------------------------------
	// Service methods.
	// -----------------------------------------------

	// ----------
	// UPDATE
	void Update(float deltaTime, float totalTime);
	void UpdatePosition(const DirectX::XMFLOAT3 request);
	void UpdateScale(const DirectX::XMFLOAT3 request);
	void UpdateRotation(const DirectX::XMFLOAT4 request);

	// ----------
	// RELATIVE TRANSFORMATION
	virtual void Move(float x, float y, float z);
	virtual void Scale(float x, float y, float z);
	virtual void Rotate(float pitchY, float yawX, float rollZ);

	// ----------
	// ABSOLUTE TRANSFORMATION
	virtual void MoveTo(float x, float y, float z);
	virtual void ScaleTo(float x, float y, float z);
	virtual void RotateTo(float pitchY, float yawX, float rollZ);

	// ----------
	// WORLD MATRIX
	void CalculateWorldMatrix(DirectX::XMFLOAT4X4& target) const;

protected:

	// ----------
	// Handle enqueued transformations.
	virtual void HandleTransformations();
	virtual void HandlePosition(const DirectX::XMFLOAT3& transformation);
	virtual void HandleScale(const DirectX::XMFLOAT3& transformation);
	virtual void HandleRotation(const DirectX::XMFLOAT4& transformation);

private:

	// -----------------------------------------------
	// Data members.
	// -----------------------------------------------

	// Stores the shared mesh.
	MeshReference sharedMesh;

	// Enqueued transformation changes. Keeps requests in the order they're received.
	TransformBuffer transformBuffer;

	// Transformation storage.
	TRANSFORM local;

	// -----------------------------------------------
	// Mutators.
	// -----------------------------------------------

	// ----------
	// POSITION
	void SetPosition(const DirectX::XMFLOAT3& source);
	void SetPosition(float x, float y, float z);
	void SetPosition(_In_reads_(3) const float *data);
	void SetPosition(const TRANSFORM& source);

	// ----------
	// SCALE
	void SetScale(const DirectX::XMFLOAT3& source);
	void SetScale(float x, float y, float z);
	void SetScale(_In_reads_(3) const float *data);
	void SetScale(const TRANSFORM& source);

	// ----------
	// ROTATION
	void SetRotation(const DirectX::XMFLOAT4& source);
	void SetRotation(float x, float y, float z, float w);
	void SetRotation(_In_reads_(4) const float *data);
	void SetRotation(const TRANSFORM& source);
	void SetRotationRollPitchYaw(float pitchY, float yawX, float rollZ);
	void SetRotationRollPitchYaw(_In_reads_(3) const float *data);

	// -----------------------------------------------
	// Helper methods.
	// -----------------------------------------------

	// Sets the default transformation values.
	void CreateTransformations();
};

