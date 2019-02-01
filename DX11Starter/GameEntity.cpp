#include "GameEntity.h"

// We'll need use this namespace for the DirectXMath library.
using namespace DirectX;

/// <summary>
/// Sets reference to shared Mesh* and sets default values for the world matrix and transformations.
/// </summary>
/// <param name="sharedMesh">Shared mesh reference.</param>
GameEntity::GameEntity(std::shared_ptr<Mesh> const& mesh)
{
	// Set the shared mesh.
	this->sharedMesh = mesh;	// Assign the parameter.

	// Create the transformations.
	CreateTransformations();
}

/// <summary>
/// Destructor, resets shared pointer.
/// </summary>
GameEntity::~GameEntity()
{
	// Meshes are handled by shared_ptr reference and will be deleted once there are no more references.
	sharedMesh.reset();
}

/// <summary>
/// Set default transformation values.
/// </summary>
void GameEntity::CreateTransformations() {

	// Set up world matrix
	// - In an actual game, each object will need one of these and they should
	//    update when/if the object moves (every frame)
	// - You'll notice a "transpose" happening below, which is redundant for
	//    an identity matrix.  This is just to show that HLSL expects a different
	//    matrix (column major vs row major) than the DirectX Math library
	XMMATRIX WM = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(WM));

	// Store the vectors.
	position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
	XMStoreFloat4(&rotation, XMQuaternionIdentity()); // Set rotation default.
}

// Accessors.

/// <summary>
/// Access the shared mesh reference.
/// </summary>
/// <returns>Returns constant shared pointer.</returns>
std::shared_ptr<Mesh> GameEntity::GetMeshReference() const {
	return sharedMesh;
}

/// <summary>
/// Fills pointer with world matrix value.
/// </summary>
/// <param name="wmDestination">Destination of field value.</param>
void GameEntity::LoadWorldMatrix(XMFLOAT4X4& wmDestination) const {
	wmDestination = worldMatrix;
	// XMStoreFloat4x4(wmDestination, XMLoadFloat4x4(&worldMatrix));
}

/// <summary>
/// Fills pointer with position value.
/// </summary>
/// <param name="wmDestination">Destination of field value.</param>
void GameEntity::LoadPosition(XMFLOAT3& posDestination) const {
	posDestination = position;
}

/// <summary>
/// Fills pointer with scale value.
/// </summary>
/// <param name="wmDestination">Destination of field value.</param>
void GameEntity::LoadScale(XMFLOAT3& scaleDestination) const {
	scaleDestination = scale;
}

/// <summary>
/// Fills pointer with rotation value.
/// </summary>
/// <param name="wmDestination">Destination of field value.</param>
void GameEntity::LoadRotation(XMFLOAT4& rotDestination) const {
	rotDestination = rotation;
}


// Mutators

/// <summary>
/// Set the transformation values.
/// </summary>
/// <param name="x">X-axis value.</param>
/// <param name="y">Y-axis value.</param>
/// <param name="z">Z-axis value.</param>
void GameEntity::SetTranslation(float x, float y, float z) {
	XMStoreFloat3(&position, XMVectorSet(x, y, z, 0));
}

/// <summary>
/// Set the transformation values.
/// </summary>
/// <param name="x">X-axis value.</param>
/// <param name="y">Y-axis value.</param>
/// <param name="z">Z-axis value.</param>
void GameEntity::SetScale(float x, float y, float z) {
	XMStoreFloat3(&scale, XMVectorSet(x, y, z, 0));
}

/// <summary>
/// Set angle of rotation for pitch (y), yaw (x), and roll (z) in radians.
/// </summary>
/// <param name="pitchY">Pitch, rotation around x-axis.</param>
/// <param name="yawX">Yaw, rotation around y-axis.</param>
/// <param name="rollZ">Roll, rotation around z-axis.</param>
void GameEntity::SetRotation(float pitchY, float yawX, float rollZ) {
	XMStoreFloat4(&rotation, XMQuaternionRotationRollPitchYaw(pitchY, yawX, rollZ));
}

// Service Methods.

void GameEntity::Update(float deltaTime, float totalTime) {

}

void GameEntity::Draw(float deltaTime, float totalTime) {

}

void GameEntity::Move(float x, float y, float z) {

}

void GameEntity::MoveForward(float distance) {

}

