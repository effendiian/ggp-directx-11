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
#include "Material.h"

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
	// Friend methods.
	// -----------------------------------------------

	friend void swap(GameEntity& lhs, GameEntity& rhs);

	// -----------------------------------------------
	// Constructors.
	// -----------------------------------------------

	// Pointer to shared mesh.
	GameEntity(Material& _material, MeshReference& sharedMesh);

	// Initialize game entity with a starting position.
	GameEntity(Material& _material, MeshReference& sharedMesh, float pX, float pY, float pZ);

	// Initialize game entity with a starting position and scale.
	GameEntity(Material& _material, MeshReference& sharedMesh,
		float pX, float pY, float pZ,
		float sX, float sY, float sZ);

	// Initialize game entity with a starting position, scale, and rotation.
	GameEntity(Material& _material, MeshReference& sharedMesh,
		float pX, float pY, float pZ,
		float sX, float sY, float sZ,
		float rX, float rY, float rZ, float rW);

	// Initialize game entity with existing Transform.
	GameEntity(Material& _material, MeshReference& sharedMesh, const TRANSFORM t);

	// Destructor.
	~GameEntity();

	// Copy/Move constructor/operator.
	GameEntity(const GameEntity& other);
	GameEntity(GameEntity&& other) noexcept;
	GameEntity& operator=(GameEntity other);

	// -----------------------------------------------
	// Static methods.
	// -----------------------------------------------

	// ----------
	// FACTORY METHODS

	// Factory method to create a single entity.
	static void CreateGameEntity(GameEntity& gameEntity, Material& material, MeshReference& sharedMesh);

	// Create specified number of game entities and append them to an existing std::vector.
	static void CreateGameEntities(GameEntityCollection& gameEntities, Material& material, MeshReference& sharedMesh, int count = 1);

	// Return a transformation between a set of bounds.
	static const float GetRandomFloat(); // Get random value between 0 to RAND_MAX.
	static const float GetRandomFloat(float min, float max); // Get random value between min and max.
	static const DirectX::XMFLOAT3 GetRandomTransform(); // 0 to RAND_MAX.
	static const DirectX::XMFLOAT3 GetRandomTransform(float min, float max); // [Inclusive min, Exclusive max)
	static const DirectX::XMFLOAT3 GetRandomTransform(DirectX::XMFLOAT3 min, DirectX::XMFLOAT3 max); // [Inclusive min, Exclusive max)

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

	// ----------
	// Material
	const Material& GetMaterial() const;

	// -----------------------------------------------
	// Mutators.
	// -----------------------------------------------

	// ----------
	// TRANSFORM

	void SetTransform(const TRANSFORM& transformation);	// Set the value of the internal transform object to the parameter.

	// ----------
	// Material

	void SetMaterial(Material& _material);
	void PrepareMaterial(DirectX::XMFLOAT4X4& _view, DirectX::XMFLOAT4X4& _projection);

	// -----------------------------------------------
	// Service methods.
	// -----------------------------------------------

	// ----------
	// UPDATE
	void Update(float deltaTime, float totalTime);
	void UpdatePosition(const DirectX::XMFLOAT3 request, TransformBuffer::TransformScope scope = TransformBuffer::S_RELATIVE);
	void UpdateScale(const DirectX::XMFLOAT3 request, TransformBuffer::TransformScope scope = TransformBuffer::S_RELATIVE);
	void UpdateRotation(const DirectX::XMFLOAT4 request, TransformBuffer::TransformScope scope = TransformBuffer::S_RELATIVE);

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
	virtual void HandlePosition(const DirectX::XMFLOAT3& transformation, TransformBuffer::TransformScope scope = TransformBuffer::S_ABSOLUTE);
	virtual void HandleScale(const DirectX::XMFLOAT3& transformation, TransformBuffer::TransformScope scope = TransformBuffer::S_ABSOLUTE);
	virtual void HandleRotation(const DirectX::XMFLOAT4& transformation, TransformBuffer::TransformScope scope = TransformBuffer::S_ABSOLUTE);

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
	
	// Material pointer.
	Material* material;

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

