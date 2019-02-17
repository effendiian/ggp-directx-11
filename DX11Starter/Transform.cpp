// -----------------------------------------------
// Include statements.
// -----------------------------------------------
#include "Transform.h"
#include <cstdlib>

// -----------------------------------------------
// Namespace for math library.
// -----------------------------------------------
using namespace DirectX;

// -----------------------------------------------
// Static methods.
// -----------------------------------------------

// ----------
// POSITION

/// <summary>
/// Assign the source transformation to the supplied TRANSFORM.
/// </summary>
/// <param name="target">TRANSFORM to modify.</param>
/// <param name="_x">X-coordinate transformation value.</param>
/// <param name="_y">Y-coordinate transformation value.</param>
/// <param name="_z">Z-coordinate transformation value.</param>
void TRANSFORM::StorePosition(TRANSFORM& target, const float _x, const float _y, const float _z)
{
	target.SetPosition(_x, _y, _z);
}

/// <summary>
/// Assign the source transformation to the supplied TRANSFORM.
/// </summary>
/// <param name="target">TRANSFORM to modify.</param>
/// <param name="sourceArray">Array containing transformation data.</param>
void TRANSFORM::StorePosition(TRANSFORM& target, _In_reads_(3) const float *sourceArray) {
	target.SetPosition(sourceArray);
}

/// <summary>
/// Assign the source transformation to the supplied TRANSFORM.
/// </summary>
/// <param name="target">TRANSFORM to modify.</param>
/// <param name="source">Value of modified transformation.</param>
void TRANSFORM::StorePosition(TRANSFORM& target, const XMFLOAT3& source) {
	target.SetPosition(source);
}

/// <summary>
/// Retrieve transformation value from source TRANSFORM and place into reference.
/// </summary>
/// <param name="target">Target of transformation data.</param>
/// <param name="source">TRANSFORM containing transformation data.</param>
void TRANSFORM::LoadPosition(XMFLOAT3& target, const TRANSFORM& source) {
	source.LoadPosition(target);
}

// ----------
// SCALE

/// <summary>
/// Assign the source transformation to the supplied TRANSFORM.
/// </summary>
/// <param name="target">TRANSFORM to modify.</param>
/// <param name="_x">X-coordinate transformation value.</param>
/// <param name="_y">Y-coordinate transformation value.</param>
/// <param name="_z">Z-coordinate transformation value.</param>
void TRANSFORM::StoreScale(TRANSFORM& target, const float _x, const float _y, const float _z)
{
	target.SetScale(_x, _y, _z);
}

/// <summary>
/// Assign the source transformation to the supplied TRANSFORM.
/// </summary>
/// <param name="target">TRANSFORM to modify.</param>
/// <param name="sourceArray">Array containing transformation data.</param>
void TRANSFORM::StoreScale(TRANSFORM& target, _In_reads_(3) const float *sourceArray) {
	target.SetScale(sourceArray);
}

/// <summary>
/// Assign the source transformation to the supplied TRANSFORM.
/// </summary>
/// <param name="target">TRANSFORM to modify.</param>
/// <param name="source">Value of modified transformation.</param>
void TRANSFORM::StoreScale(TRANSFORM& target, const XMFLOAT3& source) {
	target.SetScale(source);
}

/// <summary>
/// Retrieve transformation value from source TRANSFORM and place into reference.
/// </summary>
/// <param name="target">Target of transformation data.</param>
/// <param name="source">TRANSFORM containing transformation data.</param>
void TRANSFORM::LoadScale(XMFLOAT3& target, const TRANSFORM& source) {
	source.LoadScale(target);
}

// ----------
// ROTATION

/// <summary>
/// Assign the source transformation to the supplied TRANSFORM.
/// </summary>
/// <param name="target">TRANSFORM to modify.</param>
/// <param name="_x">X-coordinate transformation value.</param>
/// <param name="_y">Y-coordinate transformation value.</param>
/// <param name="_z">Z-coordinate transformation value.</param>
void TRANSFORM::StoreRotation(TRANSFORM& target, const float _x, const float _y, const float _z, const float _w)
{
	target.SetRotation(_x, _y, _z, _w);
}

/// <summary>
/// Assign the source transformation to the supplied TRANSFORM.
/// </summary>
/// <param name="target">TRANSFORM to modify.</param>
/// <param name="sourceArray">Array containing transformation data.</param>
void TRANSFORM::StoreRotation(TRANSFORM& target, _In_reads_(4) const float *sourceArray) {
	target.SetRotation(sourceArray);
}

/// <summary>
/// Assign the source transformation to the supplied TRANSFORM.
/// </summary>
/// <param name="target">TRANSFORM to modify.</param>
/// <param name="source">Value of modified transformation.</param>
void TRANSFORM::StoreRotation(TRANSFORM& target, const XMFLOAT4& source) {
	target.SetRotation(source);
}

/// <summary>
/// Retrieve transformation value from source TRANSFORM and place into reference.
/// </summary>
/// <param name="target">Target of transformation data.</param>
/// <param name="source">TRANSFORM containing transformation data.</param>
void TRANSFORM::LoadRotation(XMFLOAT4& target, const TRANSFORM& source) {
	source.LoadRotation(target);
}

// ----------
// WORLD MATRIX

/// <summary>
/// Calculate world matrix from source TRANSFORM and place into reference.
/// </summary>
/// <param name="target">Target of world matrix.</param>
/// <param name="source">TRANSFORM containing world matrix.</param>
void TRANSFORM::LoadWorldMatrix(XMFLOAT4X4& target, const TRANSFORM& source) {
	source.LoadWorldMatrix(target);
}


// -----------------------------------------------
// Mutators.
// -----------------------------------------------

// ----------
// POSITION

/// <summary>
/// Set transformation value.
/// </summary>
/// <param name="x">X-coordinate transformation.</param>
/// <param name="y">Y-coordinate transformation.</param>
/// <param name="z">Z-coordinate transformation.</param>
void TRANSFORM::SetPosition(float x, float y, float z) {
	this->pX = x;
	this->pY = y;
	this->pZ = z;
}

/// <summary>
/// Set transformation value.
/// </summary>
/// <param name="sourceArray">Transformation literal.</param>
void TRANSFORM::SetPosition(_In_reads_(3) const float *sourceArray) {
	this->SetPosition(sourceArray[0], sourceArray[1], sourceArray[2]);
}

/// <summary>
/// Set transformation value.
/// </summary>
/// <param name="position">Transformation literal.</param>
void TRANSFORM::SetPosition(const DirectX::XMFLOAT3& position) {
	this->SetPosition(position.x, position.y, position.z);
}

// ----------
// SCALE

/// <summary>
/// Set transformation value.
/// </summary>
/// <param name="x">X-coordinate transformation.</param>
/// <param name="y">Y-coordinate transformation.</param>
/// <param name="z">Z-coordinate transformation.</param>
void TRANSFORM::SetScale(float x, float y, float z) {
	this->sX = x;
	this->sY = y;
	this->sZ = z;
}

/// <summary>
/// Set transformation value.
/// </summary>
/// <param name="sourceArray">Transformation literal.</param>
void TRANSFORM::SetScale(_In_reads_(3) const float *sourceArray) {
	this->SetScale(sourceArray[0], sourceArray[1], sourceArray[2]);
}

/// <summary>
/// Set transformation value.
/// </summary>
/// <param name="scale">Transformation literal.</param>
void TRANSFORM::SetScale(const DirectX::XMFLOAT3& scale) {
	this->SetScale(scale.x, scale.y, scale.z);
}

// ----------
// ROTATION

/// <summary>
/// Set rotation via quaternion literal.
/// </summary>
/// <param name="x">X-coordinate transformation.</param>
/// <param name="y">Y-coordinate transformation.</param>
/// <param name="z">Z-coordinate transformation.</param>
/// <param name="w">W-coordinate, quaternion rotation complex number.</param>
void TRANSFORM::SetRotation(float x, float y, float z, float w) {
	this->rX = x;
	this->rY = y;
	this->rZ = z;
	this->rW = w;
}

/// <summary>
/// Set transformation value.
/// </summary>
/// <param name="sourceArray">Transformation literal.</param>
void TRANSFORM::SetRotation(_In_reads_(4) const float *sourceArray) {
	this->SetRotation(sourceArray[0], sourceArray[1], sourceArray[2], sourceArray[3]);
}


/// <summary>
/// Set rotation via quaternion literal.
/// </summary>
/// <param name="quaternion">Transformation literal.</param>
void TRANSFORM::SetRotation(const DirectX::XMFLOAT4& quaternion) {
	this->SetRotation(quaternion.x, quaternion.y, quaternion.z, quaternion.w);
}

// -----------------------------------------------
// Accessors.
// -----------------------------------------------


// ----------
// POSITION

/// <summary>
/// Return a read-only reference to transformation value.
/// </summary>
/// <returns>Returns read-only reference.</returns>
const XMFLOAT3 TRANSFORM::GetPosition() const {
	return XMFLOAT3(this->pX, this->pY, this->pZ);
}

/// <summary>
/// Place transformation value into target reference.
/// </summary>
/// <param name="target">Reference that will store the data from this TRANSFORM.</param>
void TRANSFORM::LoadPosition(XMFLOAT3& target) const {
	target = XMFLOAT3(this->GetPosition());
}

// ----------
// SCALE

/// <summary>
/// Return a read-only reference to transformation value.
/// </summary>
/// <returns>Returns read-only reference.</returns>
const XMFLOAT3 TRANSFORM::GetScale() const {
	return XMFLOAT3(this->sX, this->sY, this->sZ);
}

/// <summary>
/// Place transformation value into target reference.
/// </summary>
/// <param name="target">Reference that will store the data from this TRANSFORM.</param>
void TRANSFORM::LoadScale(XMFLOAT3& target) const {
	target = XMFLOAT3(this->GetScale());
}

// ----------
// ROTATION

/// <summary>
/// Return a read-only reference to transformation value.
/// </summary>
/// <returns>Returns read-only reference.</returns>
const XMFLOAT4 TRANSFORM::GetRotation() const {
	return XMFLOAT4(this->rX, this->rY, this->rZ, this->rW);
}

/// <summary>
/// Place transformation value into target reference.
/// </summary>
/// <param name="target">Reference that will store the data from this TRANSFORM.</param>
void TRANSFORM::LoadRotation(XMFLOAT4& target) const {
	target = XMFLOAT4(this->GetRotation());
}

// ----------
// WORLD MATRIX

/// <summary>
/// Return a read-only reference to transform's world matrix.
/// </summary>
/// <returns>Returns read-only reference.</returns>
const XMFLOAT4X4 TRANSFORM::GetWorldMatrix() const {
	// Get most recent transformations.
	XMFLOAT3 position = this->GetPosition();
	XMFLOAT3 scale = this->GetScale();
	XMFLOAT4 rotation = this->GetRotation();

	// Create the transformation matrices.
	XMMATRIX SCALE = XMMatrixScaling(scale.x, scale.y, scale.z);
	XMMATRIX ROTATION = XMMatrixRotationQuaternion(XMLoadFloat4(&rotation));
	XMMATRIX TRANSLATION = XMMatrixTranslation(position.x, position.y, position.z);

	// Calculate the world matrix from SRT matrix multiplication.
	XMMATRIX WMAT = SCALE * ROTATION * TRANSLATION;

	// Store and return the result.
	XMFLOAT4X4 result;
	XMStoreFloat4x4(&result, XMMatrixTranspose(WMAT)); // Transpose for HLSL.

	// Return calculated matrix.
	return result;
}

/// <summary>
/// Place transformation value into target reference.
/// </summary>
/// <param name="target">Reference that will store the data from this TRANSFORM.</param>
void TRANSFORM::LoadWorldMatrix(XMFLOAT4X4& target) const {
	target = XMFLOAT4X4(this->GetWorldMatrix());
}

// -----------------------------------------------
// Service methods.
// -----------------------------------------------

// ----------
// ROTATION

/// <summary>
/// Create and set quaternion from pitch, yaw, roll based around rotations applied in Roll * Pitch * Yaw order.
/// </summary>
/// <param name="pitchY">Pitch (+/- Y), rotation around x-axis.</param>
/// <param name="yawX">Yaw (+/- X), rotation around y-axis.</param>
/// <param name="rollZ">Roll (+/- Z), rotation around z-axis.</param>
void TRANSFORM::SetQuaternionRotationRollPitchYaw(float pitchY, float yawX, float rollZ) {
	XMFLOAT4 quaternion;
	XMStoreFloat4(&quaternion, XMQuaternionRotationRollPitchYaw(pitchY, yawX, rollZ));
	this->SetRotation(quaternion);
}

/// <summary>
/// Create and set quaternion from pitch, yaw, roll based around rotations applied in Roll * Pitch * Yaw order.
/// </summary>
/// <param name="sourceArray">Float array containing [ Pitch (y), Yaw (x), Roll (z) ].</param>
void TRANSFORM::SetQuaternionRotationRollPitchYaw(_In_reads_(3) const float *sourceArray) {
	this->SetQuaternionRotationRollPitchYaw(sourceArray[0], sourceArray[1], sourceArray[2]);
}

// ----------
// WORLD MATRIX

/// <summary>
/// Calculate world matrix of a transform using data members.
/// </summary>
/// <param name="target">Destination of world matrix data.</param>
void TRANSFORM::CalculateWorldMatrix(XMFLOAT4X4& target) const {
	target = XMFLOAT4X4(this->GetWorldMatrix());
}


