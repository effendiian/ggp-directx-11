// -----------------------------------------------
// Include statements.
// -----------------------------------------------
#include "GameEntity.h"

// -----------------------------------------------
// We'll need use this namespace for the DirectXMath library.
// -----------------------------------------------
using namespace DirectX;

// -----------------------------------------------
// -----------------------------------------------
// PUBLIC methods.
// -----------------------------------------------
// -----------------------------------------------

// -----------------------------------------------
// Constructors.
// -----------------------------------------------

// ----------
// Default constructor.

/// <summary>
/// Sets reference to shared Mesh* and sets default values for the world matrix and transformations.
/// </summary>
/// <param name="sharedMesh">Shared mesh reference.</param>
GameEntity::GameEntity(MeshReference& mesh)
	: sharedMesh(mesh), transformBuffer(TransformBuffer()), local(TRANSFORM())
{
	// Initialize members.
	this->sharedMesh = mesh;	// Assign the parameter. Increases ref count.
	this->local = TRANSFORM();
	this->CreateTransformations();  // Initialize the local.
}

// ----------
// Copy constructor.

/// <summary>
/// Copies reference from one entity to the other.
/// </summary>
/// <param name="other">Entity to copy.</param>
GameEntity::GameEntity(GameEntity& other)
	: local(TRANSFORM())
{
	// If not the same entity.
	if (this != &other) {
		// Assign another shared mesh.
		this->sharedMesh = MeshReference(nullptr); // Initialize as null.
		this->sharedMesh = other.GetMesh(); // Assign the shared pointer. Increases the ref count.
		this->local = TRANSFORM(other.GetTransform());
		this->transformBuffer = TransformBuffer(other.transformBuffer);
	}
}

// ----------
// Copy assignment.

/// <summary>
/// Copies assignment.
/// </summary>
/// <param name="other">Entity to copy.</param>
/// <returns>Returns reference to copied entity.</returns>
GameEntity& GameEntity::operator=(GameEntity other)
{
	std::swap(*this, other);
	return *this;
}

// ----------
// Special constructor(s).

/// <summary>
/// Initialize game entity with a starting position.
/// </summary>
/// <param name="sharedMesh">Shared mesh reference.</param>
/// <param name="pX">Position data.</param>
/// <param name="pY">Position data.</param>
/// <param name="pZ">Position data.</param>
GameEntity::GameEntity(MeshReference& sharedMesh,
	float pX, float pY, float pZ)
	: GameEntity(sharedMesh)
{
	// Initialize
	local.SetPosition(pX, pY, pZ);
}

/// <summary>
/// Initialize game entity with a starting position.
/// </summary>
/// <param name="sharedMesh">Shared mesh reference.</param>
/// <param name="pX">Position data.</param>
/// <param name="pY">Position data.</param>
/// <param name="pZ">Position data.</param>
/// <param name="sX">Scale data.</param>
/// <param name="sY">Scale data.</param>
/// <param name="sZ">Scale data.</param>
GameEntity::GameEntity(MeshReference& sharedMesh,
	float pX, float pY, float pZ,
	float sX, float sY, float sZ)
	: GameEntity(sharedMesh, pX, pY, pZ)
{
	// Initialize
	local.SetScale(sX, sY, sZ);
}

/// <summary>
/// Initialize game entity with a starting position.
/// </summary>
/// <param name="sharedMesh">Shared mesh reference.</param>
/// <param name="pX">Position data.</param>
/// <param name="pY">Position data.</param>
/// <param name="pZ">Position data.</param>
/// <param name="sX">Scale data.</param>
/// <param name="sY">Scale data.</param>
/// <param name="sZ">Scale data.</param>
/// <param name="rX">Rotation data.</param>
/// <param name="rY">Rotation data.</param>
/// <param name="rZ">Rotation data.</param>
/// <param name="rW">Rotation data.</param>
GameEntity::GameEntity(MeshReference& sharedMesh,
	float pX, float pY, float pZ,
	float sX, float sY, float sZ,
	float rX, float rY, float rZ, float rW)
	: GameEntity(sharedMesh, pX, pY, pZ, sX, sY, sZ)
{
	// Initialize
	local.SetRotation(rX, rY, rZ, rW);
}


/// <summary>
/// Initialize game entity with existing Transform.
/// </summary>
/// <param name="sharedMesh">Shared mesh reference.</param>
/// <param name="t">Transformation data.</param>
GameEntity::GameEntity(MeshReference& sharedMesh,
	const TRANSFORM t)
	: GameEntity(sharedMesh,
		t.pX, t.pY, t.pZ,
		t.sX, t.sY, t.sZ,
		t.rX, t.rY, t.rZ, t.rW)
{
	// No need to copy transform; it's already had its values assigned.
}

// ----------
// Destructor.

/// <summary>
/// Destructor, resets shared pointer.
/// </summary>
GameEntity::~GameEntity()
{
	// Meshes are handled by shared_ptr reference and will be deleted once there are no more references.
	sharedMesh.reset();
}

// -----------------------------------------------
// Static methods.
// -----------------------------------------------

// ----------
// FACTORY METHODS

/// <summary>
/// Factory method to create a single entity.
/// </summary>
/// <param name="gameEntity"></param>
/// <param name="sharedMesh"></param>
void GameEntity::CreateGameEntity(GameEntity& gameEntity, MeshReference& sharedMesh)
{
	// Assign a game entity to the provided reference.
	gameEntity = GameEntity(sharedMesh);
}

/// <summary>
/// Create specified number of game entities and append them to an existing std::vector.
/// </summary>
/// <param name="gameEntities">Collection of game entities.</param>
/// <param name="sharedMesh">Shared mesh reference.</param>
/// <param name="count">Number of entities to create.</param>
void GameEntity::CreateGameEntities(GameEntityCollection& gameEntities, MeshReference& sharedMesh, int count)
{
	for (int i = 0; i < count; i++) {
		gameEntities.push_back(std::make_unique<GameEntity>(sharedMesh));
	}
}

// -----------------------------------------------
// Accessors.
// -----------------------------------------------

// ----------
// TRANSFORM

/// <summary>
/// Get the local TRANSFORM object.
/// </summary>
/// <returns>A const containing a copy of the Entity's transform values.</returns>
const TRANSFORM GameEntity::GetTransform() const
{
	return local;
}

/// <summary>
/// Place the local TRANSFORM into the target.
/// </summary>
/// <param name="target">Destination of local transform.</param>
void GameEntity::LoadTransform(TRANSFORM& target) const
{
	target = this->GetTransform();
}

// ----------
// POSITION

/// <summary>
/// Get the transformation data.
/// </summary>
/// <returns>Returns copy of the transformation data.</returns>
const XMFLOAT3 GameEntity::GetPosition() const
{
	return local.GetPosition();
}

/// <summary>
/// Get the transformation data.
/// </summary>
/// <param name="target">Destination of transformation data.</param>
void GameEntity::LoadPosition(XMFLOAT3& target) const
{
	local.LoadPosition(target);
}

// ----------
// SCALE

/// <summary>
/// Get the transformation data.
/// </summary>
/// <returns>Returns copy of the transformation data.</returns>
const XMFLOAT3 GameEntity::GetScale() const
{
	return local.GetScale();
}

/// <summary>
/// Get the transformation data.
/// </summary>
/// <param name="target">Destination of transformation data.</param>
void GameEntity::LoadScale(XMFLOAT3& target) const
{
	local.LoadScale(target);
}

// ----------
// ROTATION

/// <summary>
/// Get the transformation data.
/// </summary>
/// <returns>Returns copy of the transformation data.</returns>
const XMFLOAT4 GameEntity::GetRotation() const
{
	return local.GetRotation();
}

/// <summary>
/// Get the transformation data.
/// </summary>
/// <param name="target">Destination of transformation data.</param>
void GameEntity::LoadRotation(XMFLOAT4& target) const
{
	local.LoadRotation(target);
}

// ----------
// WORLD MATRIX

/// <summary>
/// Calculate and return the world matrix.
/// </summary>
/// <returns>Returns copy of transposed (for HLSL) world matrix.</returns>
const XMFLOAT4X4 GameEntity::GetWorldMatrix() const
{
	return local.GetWorldMatrix();
}

/// <summary>
/// Get the world matrix.
/// </summary>
/// <param name="target">Destination of transformation data.</param>
void GameEntity::LoadWorldMatrix(XMFLOAT4X4& target) const
{
	local.LoadWorldMatrix(target);
}

// ----------
// MESH

/// <summary>
/// Return shared mesh reference.
/// </summary>
/// <returns>Returns reference to the shared mesh.</returns>
const GameEntity::MeshReference& GameEntity::GetMesh() const
{
	return this->sharedMesh;
}

// -----------------------------------------------
// Mutators.
// -----------------------------------------------

// ----------
// TRANSFORM

/// <summary>
/// Set the value of the internal transform object to the parameter.
/// </summary>
/// <param name="transformation">Transformation object.</param>
void GameEntity::SetTransform(const TRANSFORM& transformation)
{
	local = transformation;
}

// -----------------------------------------------
// Service methods.
// -----------------------------------------------

// ----------
// UPDATE

/// <summary>
///
/// </summary>
/// <param name="deltaTime"></param>
/// <param name="totalTime"></param>
void GameEntity::Update(float deltaTime, float totalTime)
{
	// Calculate updates based on delta time.


	// Call update/move methods based on calculations.


	// Handle transformations.
	this->HandleTransformations();
}

/// <summary>
/// Queue update for transformation.
/// </summary>
/// <param name="request">Change in transformation requested of the entity.</param>
void GameEntity::UpdatePosition(const DirectX::XMFLOAT3 request)
{
	transformBuffer.push_position(request);
}

/// <summary>
/// Queue update for transformation.
/// </summary>
/// <param name="request">Change in transformation requested of the entity.</param>
void GameEntity::UpdateScale(const DirectX::XMFLOAT3 request)
{
	transformBuffer.push_scale(request);
}

/// <summary>
/// Queue update for transformation.
/// </summary>
/// <param name="request">Change in transformation requested of the entity.</param>
void GameEntity::UpdateRotation(const DirectX::XMFLOAT4 request)
{
	transformBuffer.push_rotation(request);
}

// ----------
// RELATIVE TRANSFORMATION

/// <summary>
/// Relative transformation applied to the entity. Can be overriden.
/// </summary>
/// <param name="x">Transform data.</param>
/// <param name="y">Transform data.</param>
/// <param name="z">Transform data.</param>
void GameEntity::Move(float x, float y, float z)
{
	// Create a relative transformation based on the current transformation.
	XMFLOAT3 target(local.GetPosition()); // Get current transformation.
	target.x += x;
	target.y += y;
	target.z += z;

	// Enqueue the update.
	transformBuffer.push_position(target);
}

/// <summary>
/// Relative transformation applied to the entity. Can be overriden.
/// </summary>
/// <param name="x">Transform data.</param>
/// <param name="y">Transform data.</param>
/// <param name="z">Transform data.</param>
void GameEntity::Scale(float x, float y, float z)
{
	// Create a relative transformation based on the current transformation.
	XMFLOAT3 target(local.GetScale()); // Get current transformation.
	target.x += x;
	target.y += y;
	target.z += z;

	// Enqueue the update.
	transformBuffer.push_scale(target);
}

/// <summary>
/// Relative transformation applied to the entity. Can be overriden.
/// </summary>
/// <param name="x">Transform data.</param>
/// <param name="y">Transform data.</param>
/// <param name="z">Transform data.</param>
/// <param name="w">Transform data.</param>
void GameEntity::Rotate(float pitchY, float yawX, float rollZ)
{
	// Multiply current orientation by the RPY quaternion rotation.
	XMVECTOR orientation = XMLoadFloat4(&(local.GetRotation()));
	XMVECTOR rotation = XMQuaternionRotationRollPitchYaw(pitchY, yawX, rollZ);
	XMQuaternionMultiply(orientation, rotation);

	// Push the new orientation onto the overwrite queue.
	XMFLOAT4 target;
	XMStoreFloat4(&target, orientation);
	transformBuffer.push_rotation(target);
}

// ----------
// ABSOLUTE TRANSFORMATION

/// <summary>
/// Hard transformation that affects the entity. Can be overriden.
/// </summary>
void GameEntity::MoveTo(float x, float y, float z)
{
	transformBuffer.push_position(x, y, z);
}

/// <summary>
/// Hard transformation that affects the entity. Can be overriden.
/// </summary>
void GameEntity::ScaleTo(float x, float y, float z)
{
	transformBuffer.push_scale(x, y, z);
}

/// <summary>
/// Hard transformation that affects the entity. Can be overriden.
/// </summary>
void GameEntity::RotateTo(float pitchY, float yawX, float rollZ)
{
	XMFLOAT4 quat;
	XMStoreFloat4(&quat, XMQuaternionRotationRollPitchYaw(pitchY, yawX, rollZ));
	transformBuffer.push_rotation(quat);
}

// ----------
// WORLD MATRIX

/// <summary>
/// Returns world matrix.
/// </summary>
/// <param name="target">Destination for calculated world matrix.</param>
void GameEntity::CalculateWorldMatrix(DirectX::XMFLOAT4X4& target) const
{
	local.CalculateWorldMatrix(target);
}

// -----------------------------------------------
// -----------------------------------------------
// PRIVATE methods.
// -----------------------------------------------
// -----------------------------------------------

// -----------------------------------------------
// Mutators.
// -----------------------------------------------

// ----------
// POSITION

/// <summary>
/// Overwrite previous transformation in local transform with source.
/// </summary>
/// <param name="source">Data to write to transform.</param>
void GameEntity::SetPosition(const DirectX::XMFLOAT3& source)
{
	local.SetPosition(source);
}

/// <summary>
/// Overwrite previous transformation in local transform with source.
/// </summary>
/// <param name="x">Data to write to transform.</param>
/// <param name="y">Data to write to transform.</param>
/// <param name="z">Data to write to transform.</param>
void GameEntity::SetPosition(float x, float y, float z)
{
	this->SetPosition(XMFLOAT3(x, y, z));
}

/// <summary>
/// Overwrite previous transformation in local transform with source.
/// </summary>
/// <param name="data">Data to write to transform.</param>
void GameEntity::SetPosition(_In_reads_(3) const float *data)
{
	this->SetPosition(XMFLOAT3(data));
}

/// <summary>
/// Overwrite previous transformation in local transform with source.
/// </summary>
/// <param name="source">Data to write to transform.</param>
void GameEntity::SetPosition(const TRANSFORM& source)
{
	this->SetPosition(source.GetPosition());
}

// ----------
// SCALE

/// <summary>
/// Overwrite previous transformation in local transform with source.
/// </summary>
/// <param name="source">Data to write to transform.</param>
void GameEntity::SetScale(const DirectX::XMFLOAT3& source)
{
	local.SetScale(source);
}

/// <summary>
/// Overwrite previous transformation in local transform with source.
/// </summary>
/// <param name="x">Data to write to transform.</param>
/// <param name="y">Data to write to transform.</param>
/// <param name="z">Data to write to transform.</param>
void GameEntity::SetScale(float x, float y, float z)
{
	this->SetScale(XMFLOAT3(x, y, z));
}

/// <summary>
/// Overwrite previous transformation in local transform with source.
/// </summary>
/// <param name="data">Data to write to transform.</param>
void GameEntity::SetScale(_In_reads_(3) const float *data)
{
	this->SetScale(XMFLOAT3(data));
}

/// <summary>
/// Overwrite previous transformation in local transform with source.
/// </summary>
/// <param name="source">Data to write to transform.</param>
void GameEntity::SetScale(const TRANSFORM& source)
{
	this->SetScale(source.GetScale());
}

// ----------
// ROTATION

/// <summary>
/// Overwrite previous transformation in local transform with source.
/// </summary>
/// <param name="source">Data to write to transform.</param>
void GameEntity::SetRotation(const DirectX::XMFLOAT4& source)
{
	local.SetRotation(source);
}

/// <summary>
/// Overwrite previous transformation in local transform with source.
/// </summary>
/// <param name="x">Data to write to transform.</param>
/// <param name="y">Data to write to transform.</param>
/// <param name="z">Data to write to transform.</param>
/// <param name="w">Data to write to transform.</param>
void GameEntity::SetRotation(float x, float y, float z, float w)
{
	this->SetRotation(XMFLOAT4(x, y, z, w));
}

/// <summary>
/// Overwrite previous transformation in local transform with source.
/// </summary>
/// <param name="data">Data to write to transform.</param>
void GameEntity::SetRotation(_In_reads_(4) const float *data)
{
	this->SetRotation(XMFLOAT4(data));
}

/// <summary>
/// Overwrite previous transformation in local transform with source.
/// </summary>
/// <param name="source">Data to write to transform.</param>
void GameEntity::SetRotation(const TRANSFORM& source)
{
	this->SetRotation(source.GetRotation());
}

/// <summary>
/// Set angle of rotation for pitch (y), yaw (x), and roll (z) in radians.
/// </summary>
/// <param name="pitchY">Pitch, rotation around x-axis.</param>
/// <param name="yawX">Yaw, rotation around y-axis.</param>
/// <param name="rollZ">Roll, rotation around z-axis.</param>
void GameEntity::SetRotationRollPitchYaw(float pitchY, float yawX, float rollZ)
{
	local.SetQuaternionRotationRollPitchYaw(pitchY, yawX, rollZ);
}

/// <summary>
/// Set angle of rotation for pitch (y), yaw (x), and roll (z) in radians.
/// </summary>
/// <param name="data">Data containing [ pitch (y), yaw (x), roll (z) ] in radians.</param>
void GameEntity::SetRotationRollPitchYaw(_In_reads_(3) const float *data)
{
	local.SetQuaternionRotationRollPitchYaw(data);
}

// -----------------------------------------------
// Helper methods.
// -----------------------------------------------

// ----------
// TRANSFORM

/// <summary>
/// Set up the default transformation values.
/// </summary>
void GameEntity::CreateTransformations()
{
	// Defaults.
	local.SetPosition(0.0f, 0.0f, 0.0f);
	local.SetScale(1.0f, 1.0f, 1.0f);
	local.SetRotation(0.0f, 0.0f, 0.0f, 1.0f);
}


// -----------------------------------------------
// -----------------------------------------------
// PROTECTED methods.
// -----------------------------------------------
// -----------------------------------------------

// -----------------------------------------------
// Handle enqueued transformations.
// -----------------------------------------------

/// <summary>
/// Apply transformations.
/// </summary>
void GameEntity::HandleTransformations()
{
	// If the buffer is not empty.
	if (!transformBuffer.is_empty()) {
		while (!transformBuffer.is_empty()) {

			// Switch based on the type.
			switch (transformBuffer.peek_pair().first)
			{
			case TransformBuffer::T_POSITION:
				this->HandlePosition(transformBuffer.peek_float3());
				break;
			case TransformBuffer::T_SCALE:
				this->HandleScale(transformBuffer.peek_float3());
				break;
			case TransformBuffer::T_ROTATION:
				this->HandleRotation(transformBuffer.peek_float4());
				break;
			case TransformBuffer::T_NONE:
				break;
			default:
				break;
			}

			// Pop the next transformation off of the buffer.
			transformBuffer.pop();
		}
	}
}

/// <summary>
/// Apply transformation.
/// </summary>
/// <param name="transformation">Transformation to set.</param>
void GameEntity::HandlePosition(const XMFLOAT3& transformation)
{
	local.SetPosition(transformation);
}

/// <summary>
/// Apply transformation.
/// </summary>
/// <param name="transformation">Transformation to set.</param>
void GameEntity::HandleScale(const XMFLOAT3& transformation)
{
	local.SetScale(transformation);
}

/// <summary>
/// Apply transformation.
/// </summary>
/// <param name="transformation">Transformation to set.</param>
void GameEntity::HandleRotation(const XMFLOAT4& transformation)
{
	local.SetRotation(transformation);
}