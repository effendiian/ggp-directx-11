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

	posSpeed = XMFLOAT3(0.0f, 0.0f, 0.0f);
	rotSpeed = XMFLOAT3(0.0f, 0.0f, 0.0f);
	scaSpeed = XMFLOAT3(0.0f, 0.0f, 0.0f);
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
	XMStoreFloat3(&position, XMVectorSet(x, y, z, 0.0f));
}

/// <summary>
/// Set the transformation values.
/// </summary>
/// <param name="x">X-axis value.</param>
/// <param name="y">Y-axis value.</param>
/// <param name="z">Z-axis value.</param>
void GameEntity::SetScale(float x, float y, float z) {
	XMStoreFloat3(&scale, XMVectorSet(x, y, z, 0.0f));
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

/// <summary>
/// Set rotation via quaternion literal.
/// </summary>
/// <param name="x">X-axis value.</param>
/// <param name="y">Y-axis value.</param>
/// <param name="z">Z-axis value.</param>
/// <param name="w">W-axis value.</param>
void GameEntity::SetRotation(float x, float y, float z, float w) {
	XMStoreFloat4(&rotation, XMVectorSet(x, y, z, w));
}

/// <summary>
/// Set speed for given field.
/// </summary>
/// <param name="positionSpeed">Speed field to change.</param>
void GameEntity::SetPositionSpeed(XMFLOAT3 positionSpeed) {
	posSpeed = positionSpeed;
}

/// <summary>
/// Set speed for given field.
/// </summary>
/// <param name="rotationSpeed">Speed field to change.</param>
void GameEntity::SetRotationSpeed(XMFLOAT3 rotationSpeed) {
	rotSpeed = rotationSpeed;
}

/// <summary>
/// Set speed for given field.
/// </summary>
/// <param name="scaleSpeed">Speed field to change.</param>
void GameEntity::SetScaleSpeed(XMFLOAT3 scaleSpeed) {
	scaSpeed = scaleSpeed;
}

// Service Methods.

void GameEntity::Update(float deltaTime, float totalTime) {

	// Set transformation speeds.
	// XMFLOAT3 posSpeed(1.0f, 0.0f, 0.0f);
	// XMFLOAT3 rotSpeed(5.0f, 0.0f, 5.0f);
	// XMFLOAT3 scaSpeed(0.0f, 0.0f, 0.0f);

	// Call child update methods.
	this->UpdatePosition(deltaTime, totalTime, posSpeed);
	this->UpdateRotation(deltaTime, totalTime, rotSpeed);
	this->UpdateScale(deltaTime, totalTime, scaSpeed);

	// Prepare transformations.
	XMMATRIX TRANSLATION = XMMatrixTranslation(position.x, position.y, position.z);
	XMMATRIX ROTATION = XMMatrixRotationQuaternion(XMLoadFloat4(&rotation));
	XMMATRIX SCALE = XMMatrixScaling(scale.x, scale.y, scale.z);

	// Apply transformations.
	XMMATRIX WM = SCALE * ROTATION * TRANSLATION;
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(WM));

	// Generate a sine value by the total time.
	// float prev_angle = rotation.z;
	// float angle = deltaTime * 360.0f;
	// float magnitude = 1.0f; // Keep magnitude at 1.

	// Update some transformations.
	// this->Move(0.0f, 0.0f, sine * magnitude);
	// this->SetScale(sine * magnitude, sine * magnitude, 1.0f);
	// this->SetRotation(0.0f, 0.0f, prev_angle + XMConvertToRadians(angle * magnitude));

	// Generate a new world matrix.
	// XMVECTOR POSITION = XMLoadFloat3(&position);
	// XMVECTOR SCALE = XMLoadFloat3(&scale);
	// XMVECTOR ROTATION = XMLoadFloat4(&rotation);
	// XMVECTOR ORIGIN = XMVectorSet(0.0f, 0.0f, 0.0f, 0.0f);

	// Attempt 1
	// XMMATRIX WM = XMMatrixAffineTransformation(SCALE, ORIGIN, ROTATION, POSITION); // Create an affine transformation.
	// XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(WM)); // Transpose for HLSL.
}

/// <summary>
/// Helper function to update the position of the entity.
/// </summary>
/// <param name="deltaTime">Update delta time.</param>
/// <param name="totalTime">Total time elapsed.</param>
/// <param name="rotationSpeed">Speed of position translation in 3 planes. 0 means no change.</param>
void GameEntity::UpdatePosition(float deltaTime, float totalTime, XMFLOAT3 positionSpeed) {
	// Sine-based scaling. Change magnitude to zero to stop.
	float magnitude = 0.0f;
	float constant = deltaTime; // = deltaTime;
	float deltaSine = constant + (magnitude * deltaTime * (sinf(totalTime + deltaTime) - sinf(totalTime)));

	XMFLOAT3 deltaPosition = XMFLOAT3(
		positionSpeed.x * deltaSine,
		positionSpeed.y * deltaSine,
		positionSpeed.z * deltaSine
	);

	// Update the scale value.
	this->Move(deltaPosition.x, deltaPosition.y, deltaPosition.z);
}

/// <summary>
/// Helper function to update the rotation of the entity.
/// </summary>
/// <param name="deltaTime">Update delta time.</param>
/// <param name="totalTime">Total time elapsed.</param>
/// <param name="rotationSpeed">Speed of rotation transformation around 3 axes. 0 means no change. Speed in degrees.</param>
void GameEntity::UpdateRotation(float deltaTime, float totalTime, XMFLOAT3 rotationSpeed) {
	// Convert degrees to radians.
	XMFLOAT3 radiansPerSecond = XMFLOAT3(
		XMConvertToRadians(rotationSpeed.x),
		XMConvertToRadians(rotationSpeed.y),
		XMConvertToRadians(rotationSpeed.z)
	);

	// Sine-based rotation. Change magnitude to zero to stop.
	float magnitude = 1.0f;
	float constant = deltaTime; //= 0.0f;
	float deltaSine = constant + (magnitude * deltaTime * (sinf(totalTime + deltaTime) - sinf(totalTime)));

	// Basic rotation speed per second, detached from frame.
	XMFLOAT3 deltaRadians = XMFLOAT3(
		radiansPerSecond.x * deltaSine,
		radiansPerSecond.y * deltaSine,
		radiansPerSecond.z * deltaSine
	);

	// Calculate the new rotation quaternion.
	XMVECTOR QUATERNION = XMQuaternionMultiply(
		XMLoadFloat4(&rotation), // Current rotation.
		XMQuaternionRotationRollPitchYaw(deltaRadians.x, deltaRadians.y, deltaRadians.z) // New rotation.
	);

	// Store the calculated rotation.
	XMFLOAT4 newRotation; XMStoreFloat4(&newRotation, QUATERNION);
	this->SetRotation(newRotation.x, newRotation.y, newRotation.z, newRotation.w);
}

/// <summary>
/// Helper function to update the scale of the entity.
/// </summary>
/// <param name="deltaTime">Update delta time.</param>
/// <param name="totalTime">Total time elapsed.</param>
/// <param name="scaleSpeed">Speed of scale transformation. 0 means no change along that dimension.</param>
void GameEntity::UpdateScale(float deltaTime, float totalTime, XMFLOAT3 scaleSpeed) {
	// Sine-based scaling. Change magnitude to zero to stop.
	float magnitude = 0.0f;
	float constant = 1.0f; // = deltaTime;
	float deltaSine = constant + (magnitude * deltaTime * (sinf(totalTime + deltaTime) - sinf(totalTime)));

	// Basic scale per second, detached from frame.
	XMFLOAT3 deltaScale = XMFLOAT3(
		scaleSpeed.x * deltaSine,
		scaleSpeed.y * deltaSine,
		scaleSpeed.z * deltaSine
	);

	// Store the calculated scale.
	XMFLOAT3 newScale = XMFLOAT3(
		scale.x + deltaScale.x,
		scale.y + deltaScale.y,
		scale.z + deltaScale.z
	);

	// Update the scale value.
	this->SetScale(newScale.x, newScale.y, newScale.z);
}

void GameEntity::Move(float x, float y, float z) {
	XMFLOAT3 newPosition = XMFLOAT3(
		position.x + x,
		position.y + y,
		position.z + z
	);
	this->SetTranslation(newPosition.x, newPosition.y, newPosition.z);
}

/* void GameEntity::MoveForward(float distance) {
	// Forward vector default: (0.0f, 0.0f, 1.0f)
	// XMVECTOR FORWARD = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);
	// XMVECTOR TEMP = XMLoadFloat4(&rotation);
	// XMQuaternionMultiply(TEMP, FORWARD);

	// Normalize forward vector and scale by distance.
	// XMFLOAT4 forward;
	// XMStoreFloat4(&forward, XMVectorScale(XMVector4Normalize(TEMP), distance));

	// Translate using the scaled forward vector.
	// this->Move(forward.x, forward.y, forward.z);
} */

