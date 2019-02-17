// --------------------------------------
// Include statements.
// --------------------------------------
#include "Camera.h"
#include <stdexcept>
#include <utility>
#include <iterator>
#include <algorithm>

using namespace DirectX;

#pragma region UnitVector - Internal Camera structure.

// --------------------------------------
// UnitVector internal structure.
// --------------------------------------

// ---------------------
// Public members.
// ---------------------

// ---------------------
// Static methods.
	
/// <summary>
/// Gets the default up.
/// </summary>
/// <returns>Returns a {0, 1, 0} vector.</returns>
UnitVector UnitVector::GetDefaultUp() {
	return UnitVector(0.0f, 1.0f, 0.0f);
}

/// <summary>
/// Gets the default right.
/// </summary>
/// <returns>Returns a {1, 0, 0} vector.</returns>
UnitVector UnitVector::GetDefaultRight() {
	return UnitVector(1.0f, 0.0f, 0.0f);
}

/// <summary>
/// Gets the default forward.
/// </summary>
/// <returns>Returns a {0, 0, 1} vector.</returns>
UnitVector UnitVector::GetDefaultForward() {
	return UnitVector(0.0f, 0.0f, 1.0f);
}

// --------------------
// Friend methods.

/// <summary>
/// Swaps the specified LHS.
/// </summary>
/// <param name="lhs">The LHS.</param>
/// <param name="rhs">The RHS.</param>
void swap(UnitVector& lhs, UnitVector& rhs)
{
	using std::swap;

	swap(lhs.data, rhs.data);
}

// ---------------------
// Constructor(s).

/// <summary>
/// Initializes a new instance of the <see cref="UnitVector"/> struct.
/// </summary>
UnitVector::UnitVector()
	: UnitVector(0.0f, 0.0f, 0.0f) {}

/// <summary>
/// Constructs a special vector that only holds unit values.
/// </summary>
/// <param name="x">The x-coordinate data.</param>
/// <param name="y">The y-coordinate data.</param>
/// <param name="z">The z-coordinate data.</param>
UnitVector::UnitVector(float x, float y, float z)
	: data{ x, y, z }
{
	this->normalize();
}

/// <summary>
/// Constructs a special vector that only holds unit values.
/// </summary>
/// <param name="data">The data.</param>
UnitVector::UnitVector(_In_reads_(3) const float* source)
	: UnitVector(source[0], source[1], source[2]) {}

/// <summary>
/// Constructs a special vector that only holds unit values.
/// </summary>
/// <param name="data">The data.</param>
UnitVector::UnitVector(XMFLOAT3 data)
	: UnitVector(data.x, data.y, data.z) {}

/// <summary>
/// Finalizes an instance of the <see cref="UnitVector"/> class.
/// </summary>
UnitVector::~UnitVector() {}

/// <summary>
/// Initializes a new instance of the <see cref="UnitVector"/> struct.
/// </summary>
/// <param name="other">The other.</param>
UnitVector::UnitVector(const UnitVector& other)
{
	// Copy internal array values.
	data = other.data; // Possible due to copy assignment via std::array<T, size>.
}

/// <summary>
/// Initializes a new instance of the <see cref="UnitVector"/> struct.
/// </summary>
/// <param name="other">The other.</param>
UnitVector::UnitVector(UnitVector&& other) noexcept
	: UnitVector()
{
	// UnitVector() is initialized via initialization list (C++11).
	// References are swapped.
	// The rvalue other is a temporary and will be removed from memory after exiting scope.
	swap(*this, other);
}

/// <summary>
/// Unified copy and move assignment operator for <see cref="UnitVector"/>.
/// </summary>
/// <param name="other">The other.</param>
/// <returns>Returns reference to this instance of <see cref="UnitVector"/>.</returns>
UnitVector& UnitVector::operator=(UnitVector other)
{
	// Passed by value so a copy is made!
	// (Unless it's an rvalue, in which the compiler won't copy
	// and it will be moved in the next line, automatically).
	swap(*this, other);

	return *this;
}

// -------------
// Accessors.

/// <summary>
/// Gets this instance's internal data.
/// </summary>
/// <returns>Returns 3 float vector.</returns>
XMFLOAT3 UnitVector::Get() const {
	return XMFLOAT3(data.data());
}

/// <summary>
/// Gets the specified dimension.
/// </summary>
/// <param name="dimension">The dimension.</param>
/// <returns>Returns value at specific dimension.</returns>
float UnitVector::Get(unsigned int dimension) const
{
	try {
		return this->data.at(dimension);
	}
	catch (std::out_of_range e) 
	{
		// Further up the pipeline.
		throw std::out_of_range("Provided vector dimension is out of range.");
	}
}

/// <summary>
/// Scales the specified magnitude.
/// </summary>
/// <param name="magnitude">The magnitude.</param>
/// <returns>Returns a copy of the vector.</returns>
DirectX::XMFLOAT3 UnitVector::Scale(float magnitude) const
{
	float x, y, z;
	x = this->data[0] * magnitude;
	y = this->data[1] * magnitude;
	z = this->data[2] * magnitude;
	return XMFLOAT3(x, y, z);
}

// -------------
// Mutators.

/// <summary>
/// Sets the specified vector.
/// </summary>
/// <param name="x">The x.</param>
/// <param name="y">The y.</param>
/// <param name="z">The z.</param>
void UnitVector::Set(float x, float y, float z)
{
	this->data = { x, y, z };
	this->normalize();
}

/// <summary>
/// Sets the specified vector.
/// </summary>
/// <param name="data">The data.</param>
void UnitVector::Set(_In_reads_(3) const float* source)
{
	return this->Set(source[0], source[1], source[2]);
}

/// <summary>
/// Sets the specified vector.
/// </summary>
/// <param name="vector">The vector.</param>
void UnitVector::Set(DirectX::XMFLOAT3 vector)
{
	return this->Set(vector.x, vector.y, vector.z);
}

// ---------------------
// Private members.
// ---------------------

// -------------
// Helper methods.

/// <summary>
/// Normalizes this vector.
/// </summary>
void UnitVector::normalize() {

	// Get the values.
	float x = this->data[0];
	float y = this->data[1];
	float z = this->data[2];

	// Calculate the magnitude.
	float x2 = pow(x, 2);
	float y2 = pow(y, 2);
	float z2 = pow(z, 2);
	float mag = sqrt(x2 + y2 + z2);

	// Calculate the unit vector values.
	float ux = x / mag;
	float uy = y / mag;
	float uz = z / mag;

	// Set the values.
	this->data = { ux, uy, uz };
}

#pragma endregion

#pragma region TransformDescription - Internal transformation data.

// --------------------------------------
// TransformDescription internal structure.
// --------------------------------------

// ---------------------
// Public members.
// ---------------------

// ---------------------
// Static methods.

/// <summary>
/// Gets the default transform.
/// </summary>
/// <returns>Returns the default TransformDescription.</returns>
TransformDescription TransformDescription::GetDefaultTransform()
{
	return TransformDescription();
}

// --------------------
// Friend methods.

void swap(TransformDescription& lhs, TransformDescription& rhs) 
{
	using std::swap;
	
	// Swap the members.
	swap(lhs.heading, rhs.heading);
	swap(lhs.up, rhs.up);
	swap(lhs.position_start, rhs.position_start);
	swap(lhs.position, rhs.position);
	swap(lhs.orientation_start, rhs.orientation_start);
	swap(lhs.orientation, rhs.orientation);
}

// ---------------------
// Constructor(s).

/// <summary>
/// Creates Transform description.
/// </summary>
/// <returns>Returns a TransformDescription.</returns>
TransformDescription::TransformDescription() 
	: TransformDescription(
		XMFLOAT3(0.0f, 0.0f, 0.0f), 
		XMFLOAT3(0.0f, 0.0f, 0.0f)
	) {}

/// <summary>
/// Initializes a new instance of the <see cref="TransformDescription"/> struct.
/// </summary>
/// <param name="_position">The position.</param>
TransformDescription::TransformDescription(XMFLOAT3 _position)
	: TransformDescription(
		_position,
		XMFLOAT3(0.0f, 0.0f, 0.0f)
	) {}

/// <summary>
/// Initializes a new instance of the <see cref="TransformDescription"/> struct.
/// </summary>
/// <param name="_position">The position.</param>
/// <param name="_rotation">The rotation.</param>
TransformDescription::TransformDescription(
	XMFLOAT3 _position,
	XMFLOAT3 _rotation)
	: heading{ UnitVector::GetDefaultForward() },
	up{ UnitVector::GetDefaultUp() },
	position_start(_position), position(_position),
	orientation_start(_rotation), orientation(_rotation)
{
	this->CalculateHeading();
	this->CalculateUp();
}

/// <summary>
/// Initializes a new instance of the <see cref="TransformDescription"/> struct.
/// </summary>
/// <param name="pX">The p x.</param>
/// <param name="pY">The p y.</param>
/// <param name="pZ">The p z.</param>
/// <param name="rX">The r x.</param>
/// <param name="rY">The r y.</param>
/// <param name="rZ">The r z.</param>
TransformDescription::TransformDescription(
	float pX, float pY, float pZ,
	float rX, float rY, float rZ)
	: TransformDescription(XMFLOAT3(pX, pY, pZ), XMFLOAT3(rX, rY, rZ)) {}

/// <summary>
/// Initializes a new instance of the <see cref="TransformDescription"/> struct.
/// </summary>
/// <param name="position">The position.</param>
/// <param name="orientation">The orientation.</param>
TransformDescription::TransformDescription(
	_In_reads_(3) const float* _position,
	_In_reads_(3) const float* _rotation)
	: TransformDescription(
		XMFLOAT3(_position),
		XMFLOAT3(_rotation)
	) {}

/// <summary>
/// Finalizes an instance of the <see cref="TransformDescription"/> class.
/// </summary>
TransformDescription::~TransformDescription() {}

/// <summary>
/// Initializes a new instance of the <see cref="TransformDescription"/> struct.
/// </summary>
/// <param name="other">The other.</param>
TransformDescription::TransformDescription(const TransformDescription& other)
{
	// Assign member data.
	this->heading = other.heading;
	this->up = other.up;
	this->position_start = other.position_start;
	this->position = other.position;
	this->orientation_start = other.orientation_start;
	this->orientation = other.orientation;
}

/// <summary>
/// Initializes a new instance of the <see cref="TransformDescription"/> struct.
/// </summary>
/// <param name="other">The other.</param>
TransformDescription::TransformDescription(TransformDescription&& other) noexcept
	: TransformDescription() // Initialize for move.
{
	// Swap the temporary with the instance.
	swap(*this, other);
}

/// <summary>
/// Unified move and copy assignment operator.
/// </summary>
/// <param name="other">The other.</param>
/// <returns>Returns reference to move/copy.</returns>
TransformDescription& TransformDescription::operator=(TransformDescription other)
{
	swap(*this, other);
	return *this;
}

// ---------------------
// Accessors.

/// <summary>
/// Gets the starting position.
/// </summary>
/// <returns>Returns float.</returns>
XMFLOAT3 TransformDescription::GetStartingPosition() const 
{
	return this->position_start;
}

/// <summary>
/// Gets the starting position.
/// </summary>
/// <param name="target">The target.</param>
void TransformDescription::GetStartingPosition(XMFLOAT3& target) const
{
	target = XMFLOAT3(position_start);
}

/// <summary>
/// Gets the current position.
/// </summary>
/// <returns>Returns float.</returns>
XMFLOAT3 TransformDescription::GetCurrentPosition() const
{
	return this->position;
}

/// <summary>
/// Gets the current position.
/// </summary>
/// <param name="target">The target.</param>
void TransformDescription::GetCurrentPosition(XMFLOAT3& target) const
{
	target = XMFLOAT3(position);
}

/// <summary>
/// Gets the starting orientation.
/// </summary>
/// <returns>Returns float.</returns>
XMFLOAT3 TransformDescription::GetBaseOrientation() const
{
	return this->orientation;
}

/// <summary>
/// Gets the starting orientation.
/// </summary>
/// <param name="target">The target.</param>
void TransformDescription::GetBaseOrientation(XMFLOAT3& target) const
{
	target = XMFLOAT3(orientation);
}

/// <summary>
/// Gets the current orientation.
/// </summary>
/// <returns>Returns float.</returns>
XMFLOAT3 TransformDescription::GetCurrentOrientation() const
{
	return this->position;
}

/// <summary>
/// Gets the current orientation.
/// </summary>
/// <param name="target">The target.</param>
void TransformDescription::GetCurrentOrientation(XMFLOAT3& target) const
{
	target = XMFLOAT3(orientation);
}

/// <summary>
/// Gets the current direction.
/// </summary>
/// <returns>Returns float vector.</returns>
XMFLOAT3 TransformDescription::GetHeading() const
{
	return this->heading.Get();
}

/// <summary>
/// Gets the current direction.
/// </summary>
/// <param name="target">The target.</param>
void TransformDescription::GetHeading(XMFLOAT3& target) const
{
	target = XMFLOAT3(heading.Get());
}

/// <summary>
/// Gets the current up direction.
/// </summary>
/// <param name="target">The target.</param>
XMFLOAT3 TransformDescription::GetUp() const
{
	return this->up.Get();
}

/// <summary>
/// Gets the current up direction.
/// </summary>
/// <param name="target">The target.</param>
void TransformDescription::GetUp(XMFLOAT3& target) const
{
	target = XMFLOAT3(up.Get());
}


// ---------------------
// Mutators.

/// <summary>
/// Translates the specified direction.
/// </summary>
/// <param name="direction">The direction.</param>
/// <param name="scale">The scale.</param>
void TransformDescription::Translate(UnitVector direction, float scale)
{
	this->Translate(direction.Scale(scale));
}

/// <summary>
/// Translates position by delta.
/// </summary>
/// <param name="delta">The delta.</param>
void TransformDescription::Translate(XMFLOAT3 delta)
{
	position.x += delta.x;
	position.y += delta.y;
	position.z += delta.z;
}

/// <summary>
/// Rotates by the specified delta.
/// </summary>
/// <param name="delta">The delta.</param>
void TransformDescription::Rotate(DirectX::XMFLOAT3 delta)
{
	orientation.x += delta.x;
	orientation.y += delta.y;
	orientation.z += delta.z;
	CalculateHeading();
	CalculateUp();
}

/// <summary>
/// Sets the position.
/// </summary>
/// <param name="absolute">The vector data.</param>
void TransformDescription::SetPosition(XMFLOAT3 absolute)
{
	position = XMFLOAT3(absolute);
}

/// <summary>
/// Sets the rotation.
/// </summary>
/// <param name="absolute">The vector data.</param>
void TransformDescription::SetRotation(XMFLOAT3 absolute)
{
	orientation = XMFLOAT3(absolute);
	CalculateHeading();
	CalculateUp();
}

// -------------
// Service methods.

/// <summary>
/// Resets this instance.
/// </summary>
void TransformDescription::Reset()
{
	this->SetPosition(position_start);
	this->SetRotation(orientation_start);
}

/// <summary>
/// Updates the position.
/// </summary>
/// <param name="speed">The speed.</param>
/// <param name="isRelative">if set to <c>true</c> [is relative].</param>
void TransformDescription::UpdatePosition(
	XMFLOAT3 speed, bool isRelative /* = true */)
{
	if (speed.x + speed.y + speed.z != 0.0f) {
		XMFLOAT3 delta = XMFLOAT3(speed);

		if (!isRelative)
		{
			this->SetPosition(delta);
		}
		else
		{
			this->Translate(delta);
		}
	}
}

/// <summary>
/// Updates the rotation.
/// </summary>
/// <param name="speed">The rotation speed.</param>
/// <param name="isRelative">if set to <c>true</c> [is relative].</param>
void TransformDescription::UpdateRotation(
	XMFLOAT3 speed, bool isRelative /* = true */)
{
	if (speed.x + speed.y + speed.z != 0.0f) {
		XMFLOAT3 delta = XMFLOAT3(speed);

		if (!isRelative)
		{
			this->SetRotation(delta);
		}
		else
		{
			this->Rotate(delta);
		}

		CalculateHeading();
		CalculateUp();
	}
}

/// <summary>
/// Updates the position.
/// </summary>
/// <param name="deltaTime">The delta time.</param>
/// <param name="totalTime">The total time.</param>
/// <param name="speed">The speed.</param>
/// <param name="isRelative">if set to <c>true</c> [is relative].</param>
void TransformDescription::UpdatePosition(
	float deltaTime, float totalTime,
	XMFLOAT3 speed, bool isRelative /* = true */) 
{
	if (speed.x + speed.y + speed.z != 0.0f) {
		float dx = speed.x * deltaTime;
		float dy = speed.y * deltaTime;
		float dz = speed.z * deltaTime;
		XMFLOAT3 delta = XMFLOAT3(dx, dy, dz);

		if (!isRelative)
		{
			this->SetPosition(delta);
		}
		else
		{
			this->Translate(delta);
		}
	}
}

/// <summary>
/// Updates the rotation.
/// </summary>
/// <param name="deltaTime">The delta time.</param>
/// <param name="totalTime">The total time.</param>
/// <param name="speed">The rotation speed.</param>
/// <param name="isRelative">if set to <c>true</c> [is relative].</param>
void TransformDescription::UpdateRotation(
	float deltaTime, float totalTime, 
	XMFLOAT3 speed, bool isRelative /* = true */) 
{
	if (speed.x + speed.y + speed.z != 0.0f) {
		float dx = speed.x * deltaTime;
		float dy = speed.y * deltaTime;
		float dz = speed.z * deltaTime;
		XMFLOAT3 delta = XMFLOAT3(dx, dy, dz);

		if (!isRelative)
		{
			this->SetRotation(delta);
		}
		else
		{
			this->Rotate(delta);
		}

		CalculateHeading();
		CalculateUp();
	}
}

// ---------------------
// Private members.
// ---------------------

// ---------------------
// Helper methods.

/// <summary>
/// Calculates the heading.
/// </summary>
void TransformDescription::CalculateHeading()
{
	// Get default Forward (Z) vector.
	XMFLOAT3 forward_safe = UnitVector::GetDefaultForward().Get();
	XMVECTOR forward = XMLoadFloat3(&forward_safe);

	// Get the current orientation as a quaternion. Pitch(y), Yaw(x), Roll(z)
	XMVECTOR rotation = XMQuaternionRotationRollPitchYaw(
		this->orientation.y,
		this->orientation.x,
		this->orientation.z
	);
	
	// Apply current orientation to forward default.
	XMVECTOR direction = XMVector3Rotate(forward, rotation);
	
	// Store the direction in a safe format.
	XMFLOAT3 direction_safe;
	XMStoreFloat3(&direction_safe, direction);

	// Store the calculated result.
	this->heading.Set(direction_safe);
}

/// <summary>
/// Calculates the up vector.
/// </summary>
void TransformDescription::CalculateUp()
{
	// Get default Up (Y) vector.
	XMFLOAT3 global_up_safe = UnitVector::GetDefaultUp().Get();
	XMVECTOR global_up = XMLoadFloat3(&global_up_safe);

	// Get the current orientation as a quaternion. Pitch(y), Yaw(x), Roll(z)
	XMVECTOR rotation = XMQuaternionRotationRollPitchYaw(
		this->orientation.y,
		this->orientation.x,
		this->orientation.z
	);

	// Apply current orientation to forward default.
	XMVECTOR relative_up = XMVector3Rotate(global_up, rotation);

	// Store the direction in a safe format.
	XMFLOAT3 relative_up_safe;
	XMStoreFloat3(&relative_up_safe, relative_up);

	// Store the calculated result.
	this->up.Set(relative_up_safe);
}
#pragma endregion

#pragma region CameraOptions - Camera settings.

// --------------------------------------
// CameraOptions internal structure.
// --------------------------------------

// ---------------------
// Public members.
// ---------------------

// ---------------------
// Static methods.

/// <summary>
/// Gets the default camera options.
/// </summary>
/// <returns>Returns the default object.</returns>
CameraOptions CameraOptions::GetDefaultCameraOptions()
{
	return CameraOptions();
}

// ---------------------
// Friend methods.

/// <summary>
/// Swaps the specified LHS.
/// </summary>
/// <param name="lhs">The LHS.</param>
/// <param name="rhs">The RHS.</param>
void swap(CameraOptions& lhs, CameraOptions& rhs)
{
	using std::swap;

	// Swap the members.
	swap(lhs.fieldOfView, rhs.fieldOfView);
	swap(lhs.width, rhs.width);
	swap(lhs.height, rhs.height);
	swap(lhs.aspectRatio, rhs.aspectRatio);
	swap(lhs.nearPlane, rhs.nearPlane);
	swap(lhs.farPlane, rhs.farPlane);
}

// ---------------------
// Constructor(s).

/// <summary>
/// Initializes a new instance of the <see cref="CameraOptions"/> struct.
/// </summary>
CameraOptions::CameraOptions()
	: CameraOptions(
		0.25f, // Default field of view angle. (It's multiplied by PI). In radians.
		1280, // Default width.
		720, // Default height.
		0.1f, // Default near clipping plane distance.
		100.0f // Default far clipping plane distance.
	) {}

/// <summary>
/// Initializes a new instance of the <see cref="CameraOptions"/> struct.
/// </summary>
/// <param name="fov">The fov.</param>
/// <param name="w">The w.</param>
/// <param name="h">The h.</param>
/// <param name="nearClip">The near clip.</param>
/// <param name="farClip">The far clip.</param>
CameraOptions::CameraOptions(float fov, float w, float h, float nearClip, float farClip) 
	: fieldOfView{ fov }, width { w }, height{ h }, aspectRatio{ (float) w/h }, nearPlane { nearClip }, farPlane { farClip }
{
	this->SetFieldOfView(fov);
	this->SetWidth(w);
	this->SetHeight(h);
	this->SetNearClippingPlane(nearClip);
	this->SetFarClippingPlane(farClip);
}

/// <summary>
/// Finalizes an instance of the <see cref="CameraOptions"/> class.
/// </summary>
CameraOptions::~CameraOptions() {}

/// <summary>
/// Initializes a new instance of the <see cref="CameraOptions"/> struct.
/// </summary>
/// <param name="other">The other.</param>
CameraOptions::CameraOptions(const CameraOptions& other)
{
	// Copy member data.
	this->fieldOfView = other.fieldOfView;
	this->width = other.width;
	this->height = other.height;
	this->aspectRatio = other.aspectRatio;
	this->nearPlane = other.nearPlane;
	this->farPlane = other.farPlane;
}

/// <summary>
/// Initializes a new instance of the <see cref="CameraOptions"/> struct.
/// </summary>
/// <param name="other">The other.</param>
CameraOptions::CameraOptions(CameraOptions&& other) noexcept
	: CameraOptions() // Initialize default to prep for move.
{
	swap(*this, other);
}

/// <summary>
/// Unified copy and move assignment operator.
/// </summary>
/// <param name="other">The other.</param>
/// <returns>Returns reference to this instance.</returns>
CameraOptions& CameraOptions::operator=(CameraOptions other)
{
	swap(*this, other);
	return *this;
}

// ---------------------
// Accessors.

/// <summary>
/// Gets the property.
/// </summary>
/// <returns>Returns property as float.</returns>
float CameraOptions::GetFieldOfView() const
{
	return (this->fieldOfView / PI);
}

/// <summary>
/// Gets the property.
/// </summary>
/// <returns>Returns property as float.</returns>
float CameraOptions::GetWidth() const
{
	return this->width;
}

/// <summary>
/// Gets the property.
/// </summary>
/// <returns>Returns property as float.</returns>
float CameraOptions::GetHeight() const
{
	return this->height;
}

/// <summary>
/// Gets the property.
/// </summary>
/// <returns>Returns property as float.</returns>
float CameraOptions::GetAspectRatio() const
{
	return this->aspectRatio;
}

/// <summary>
/// Gets the property.
/// </summary>
/// <returns>Returns property as float.</returns>
float CameraOptions::GetNearClippingPlane() const
{
	return this->nearPlane;
}

/// <summary>
/// Gets the property.
/// </summary>
/// <returns>Returns property as float.</returns>
float CameraOptions::GetFarClippingPlane() const
{
	return this->farPlane;
}

// ---------------------
// Mutators.

/// <summary>
/// Sets the field of view.
/// </summary>
/// <param name="value">The value.</param>
void CameraOptions::SetFieldOfView(float value)
{
	this->fieldOfView = value * PI;
}

/// <summary>
/// Sets the width.
/// </summary>
/// <param name="value">The value.</param>
void CameraOptions::SetWidth(float value)
{
	this->width = value;
}

/// <summary>
/// Sets the width by aspect ratio.
/// </summary>
/// <param name="ratio">The ratio.</param>
/// <param name="height">The height.</param>
void CameraOptions::SetWidthByAspectRatio(float ratio, float height)
{
	this->width = ratio * height;
	this->height = height;
}

/// <summary>
/// Sets the height.
/// </summary>
/// <param name="value">The value.</param>
void CameraOptions::SetHeight(float value)
{
	this->height = value;
}

/// <summary>
/// Sets the height by aspect ratio.
/// </summary>
/// <param name="ratio">The ratio.</param>
/// <param name="width">The width.</param>
void CameraOptions::SetHeightByAspectRatio(float ratio, float width)
{
	this->width = width;
	this->height = (float)(width / ratio);
}

/// <summary>
/// Sets the near clipping plane.
/// </summary>
/// <param name="value">The value.</param>
void CameraOptions::SetNearClippingPlane(float value)
{
	this->nearPlane = value;
}

/// <summary>
/// Sets the far clipping plane.
/// </summary>
/// <param name="value">The value.</param>
void CameraOptions::SetFarClippingPlane(float value)
{
	this->farPlane = value;
}

// ---------------------
// Service methods.

/// <summary>
/// Updates the aspect ratio.
/// </summary>
void CameraOptions::UpdateAspectRatio()
{
	this->aspectRatio = (float)((float) this->width / (float) this->height);
}

#pragma endregion

#pragma region MouseTracker - Captures horizontal/vertical deltas.

// --------------------------------------
// --------------------------------------
// Public.
// --------------------------------------
// --------------------------------------

// --------------------------------------
// Friend methods.
// --------------------------------------

/// <summary>
/// Swaps the specified LHS.
/// </summary>
/// <param name="lhs">The LHS.</param>
/// <param name="rhs">The RHS.</param>
void swap(MouseTracker& lhs, MouseTracker& rhs)
{
	using std::swap;

	// Swap members.
	swap(lhs.previousButtonState, rhs.previousButtonState);
	swap(lhs.currentButtonState, rhs.currentButtonState);
	swap(lhs.previousMousePosition, rhs.previousMousePosition);
	swap(lhs.currentMousePosition, rhs.currentMousePosition);
}


/// <summary>
/// Clamps the specified n.
/// </summary>
/// <param name="n">The n.</param>
/// <param name="low">The low.</param>
/// <param name="high">The high.</param>
/// <returns>Returns clamped value.</returns>
template <typename T>
T clamp(const T& n, const T& low, const T& high) 
{
	return std::max(low, std::min(n, high));
}

//// <summary>
/// Clamps the specified n.
/// </summary>
/// <param name="n">The n.</param>
/// <param name="originalLow">The original low.</param>
/// <param name="originalHigh">The original high.</param>
/// <param name="newLow">The new low.</param>
/// <param name="newHigh">The new high.</param>
/// <returns>Returns value within given range.</returns>
template <typename T>
T range(const T& n, const T& originalLow, const T& originalHigh, const T& newLow, const T& newHigh) 
{
	T slope = (newHigh - newLow) / (originalHigh - originalLow);
	return newLow + floor(slope * (n - originalLow) + 0.5f);
}

// --------------------------------------
// Constructor(s).
// --------------------------------------

/// <summary>
/// Constructs the MouseTracker.
/// </summary>
MouseTracker::MouseTracker()
	: previousButtonState{ false },
	currentButtonState{ false },
	previousMousePosition(0.0f, 0.0f),
	currentMousePosition(0.0f, 0.0f) {}

/// <summary>
/// Initializes a new instance of the <see cref="MouseTracker"/> struct.
/// </summary>
/// <param name="other">The other.</param>
MouseTracker::MouseTracker(const MouseTracker& other) 
{
	// Copy member data.
	this->previousButtonState = other.previousButtonState;
	this->currentButtonState = other.currentButtonState;
	this->previousMousePosition = other.previousMousePosition;
	this->currentMousePosition = other.currentMousePosition;
}

/// <summary>
/// Initializes a new instance of the <see cref="MouseTracker"/> struct.
/// </summary>
/// <param name="other">The other.</param>
MouseTracker::MouseTracker(MouseTracker&& other) noexcept
	: MouseTracker() // Initialize.
{
	swap(*this, other);
}

/// <summary>
/// Unified copy and move assignment constructor.
/// </summary>
/// <param name="other">The other.</param>
/// <returns>Returns reference to the MouseTracker.</returns>
MouseTracker& MouseTracker::operator=(MouseTracker other)
{
	swap(*this, other);
	return *this;
}

// --------------------------------------
// Accessors.
// --------------------------------------

/// <summary>
/// Gets the state of the previous button.
/// </summary>
/// <returns>Returns boolean.</returns>
bool MouseTracker::GetPreviousButtonState() const
{
	return this->previousButtonState;
}

/// <summary>
/// Gets the previous mouse position.
/// </summary>
/// <returns>Returns 2D vector.</returns>
const XMFLOAT2& MouseTracker::GetPreviousMousePosition() const
{
	return this->previousMousePosition;
}

/// <summary>
/// Gets the state of the current button.
/// </summary>
/// <returns>Returns boolean.</returns>
bool MouseTracker::GetCurrentButtonState() const
{
	return this->currentButtonState;
}

/// <summary>
/// Gets the current mouse position.
/// </summary>
/// <returns>Returns 2D vector.</returns>
const XMFLOAT2& MouseTracker::GetCurrentMousePosition() const
{
	return this->currentMousePosition;
}

/// <summary>
/// Gets the raw delta.
/// </summary>
/// <returns>Returns 2D vector.</returns>
XMFLOAT2 MouseTracker::GetRawDelta() const
{
	return this->CalculateDelta();
}

/// <summary>
/// Gets the raw horizontal delta.
/// </summary>
/// <returns>Returns float.</returns>
float MouseTracker::GetRawHorizontalDelta() const
{
	return GetRawDelta().x;
}

/// <summary>
/// Gets the raw vertical delta.
/// </summary>
/// <returns>Returns float.</returns>
float MouseTracker::GetRawVerticalDelta() const
{
	return GetRawDelta().y;
}

/// <summary>
/// Gets the delta between 0 and 1.
/// </summary>
/// <param name="_min">The minimum.</param>
/// <param name="_max">The maximum.</param>
/// <returns></returns>
XMFLOAT2 MouseTracker::GetDelta(XMFLOAT2 _min, XMFLOAT2 _max) const
{
	float x = GetHorizontalDelta(_min.x, _max.x);
	float y = GetVerticalDelta(_min.y, _max.y);

	return XMFLOAT2(x, y);
}

/// <summary>
/// Gets the horizontal delta.
/// </summary>
/// <param name="_min">The minimum.</param>
/// <param name="_max">The maximum.</param>
/// <returns>Returns float.</returns>
float MouseTracker::GetHorizontalDelta(float _min, float _max) const
{
	return range(this->CalculateDeltaHorizontal(), _min, _max, 0.0f, 1.0f);
}

/// <summary>
/// Gets the vertical delta.
/// </summary>
/// <param name="_min">The minimum.</param>
/// <param name="_max">The maximum.</param>
/// <returns>Returns float.</returns>
float MouseTracker::GetVerticalDelta(float _min, float _max) const
{
	return range(this->CalculateDeltaVertical(), _min, _max, 0.0f, 1.0f);
}

// --------------------------------------
// Service methods.
// --------------------------------------

/// <summary>
/// Updates the specified current state.
/// </summary>
/// <param name="_currentState">if set to <c>true</c> [current state].</param>
/// <param name="_currentPosition">The current position.</param>
void MouseTracker::Update(bool _currentState, XMFLOAT2 _currentPosition)
{
	// Overwrites previous button state and position.
	this->SetPreviousButtonState(this->currentButtonState);
	this->SetPreviousMousePosition(this->currentMousePosition);

	// Update the current state and position.
	this->SetCurrentButtonState(_currentState);
	this->SetCurrentMousePosition(_currentPosition);
}

/// <summary>
/// Updates the specified current state.
/// </summary>
/// <param name="_currentState">if set to <c>true</c> [current state].</param>
/// <param name="_mouseX">The mouse x.</param>
/// <param name="_mouseY">The mouse y.</param>
void MouseTracker::Update(bool _currentState, float _mouseX, float _mouseY)
{
	this->Update(_currentState, XMFLOAT2(_mouseX, _mouseY));
}

// --------------------------------------
// --------------------------------------
// Private.
// --------------------------------------
// --------------------------------------

// --------------------------------------
// Mutators.
// --------------------------------------

/// <summary>
/// Sets the state of the previous button.
/// </summary>
/// <param name="_state">if set to <c>true</c> [state].</param>
void MouseTracker::SetPreviousButtonState(bool _state)
{
	this->previousButtonState = _state;
}

/// <summary>
/// Sets the state of the current button.
/// </summary>
/// <param name="_state">if set to <c>true</c> [state].</param>
void MouseTracker::SetCurrentButtonState(bool _state) 
{
	this->currentButtonState = _state;
}

/// <summary>
/// Sets the previous mouse position.
/// </summary>
/// <param name="_position">The position.</param>
void MouseTracker::SetPreviousMousePosition(XMFLOAT2 _position)
{
	this->previousMousePosition = _position;
}

/// <summary>
/// Sets the current mouse position.
/// </summary>
/// <param name="_position">The position.</param>
void MouseTracker::SetCurrentMousePosition(XMFLOAT2 _position)
{
	this->currentMousePosition = _position;
}

// --------------------------------------
// Helper methods.
// --------------------------------------

/// <summary>
/// Calculates the delta.
/// </summary>
/// <returns>Returns delta XY in a 2D vector.</returns>
XMFLOAT2 MouseTracker::CalculateDelta() const
{
	return XMFLOAT2(
		this->CalculateDeltaHorizontal(),
		this->CalculateDeltaVertical()
	);
}

/// <summary>
/// Calculates the delta vertical.
/// </summary>
/// <returns>Returns delta X.</returns>
float MouseTracker::CalculateDeltaHorizontal() const
{
	float pX = this->previousMousePosition.x;
	float cX = this->currentMousePosition.x;
	return (cX - pX);
}

/// <summary>
/// Calculates the delta vertical.
/// </summary>
/// <returns>Returns delta Y.</returns>
float MouseTracker::CalculateDeltaVertical() const
{
	float pY = this->previousMousePosition.y;
	float cY = this->currentMousePosition.y;
	return (cY - pY);
}

#pragma endregion

#pragma region Camera - Camera object definition/implementation.

// --------------------------------------
// Camera class.
// --------------------------------------

// ---------------------
// Public members.
// ---------------------

// ---------------------
// Static methods.

/// <summary>
/// Gets the default camera.
/// </summary>
/// <returns>Returns camera with default options.</returns>
Camera Camera::GetDefaultCamera()
{
	return Camera(CameraOptions::GetDefaultCameraOptions());
}

// ---------------------
// Friend methods.

/// <summary>
/// Swaps the specified LHS.
/// </summary>
/// <param name="lhs">The LHS.</param>
/// <param name="rhs">The RHS.</param>
void swap(Camera& lhs, Camera& rhs)
{
	using std::swap;

	// Swap members.
	swap(lhs.transform, rhs.transform);
	swap(lhs.settings, rhs.settings);
	swap(lhs.view, rhs.view);
	swap(lhs.projection, rhs.projection);
	swap(lhs.tracker, rhs.tracker);
}

// ---------------------
// Constructor(s).

/// <summary>
/// Initializes a new instance of the <see cref="Camera"/> class.
/// </summary>
Camera::Camera() : Camera(CameraOptions::GetDefaultCameraOptions()) {}

/// <summary>
/// Initializes a new instance of the <see cref="Camera"/> class.
/// </summary>
/// <param name="options">The options.</param>
Camera::Camera(CameraOptions options)
	: Camera(options, TransformDescription::GetDefaultTransform()) {}

/// <summary>
/// Initializes a new instance of the <see cref="Camera"/> class.
/// </summary>
/// <param name="options">The options.</param>
/// <param name="transform">The transform.</param>
Camera::Camera(CameraOptions options, TransformDescription transform)
	: settings{ options }, transform{ transform },
	  view(), projection(), tracker() {}

/// <summary>
/// Initializes a new instance of the <see cref="Camera"/> class.
/// </summary>
/// <param name="options">The options.</param>
/// <param name="position">The position.</param>
Camera::Camera(CameraOptions options, DirectX::XMFLOAT3 position)
	: Camera(options, TransformDescription(position)) {}

/// <summary>
/// Initializes a new instance of the <see cref="Camera"/> class.
/// </summary>
/// <param name="options">The options.</param>
/// <param name="position">The position.</param>
/// <param name="rotation">The rotation.</param>
Camera::Camera(
	CameraOptions options,
	DirectX::XMFLOAT3 position,
	DirectX::XMFLOAT3 rotation
) : Camera(options, TransformDescription(position, rotation)) {}

/// <summary>
/// Initializes a new instance of the <see cref="Camera"/> class.
/// </summary>
/// <param name="options">The options.</param>
/// <param name="pX">The p x.</param>
/// <param name="pY">The p y.</param>
/// <param name="pZ">The p z.</param>
/// <param name="rX">The r x.</param>
/// <param name="rY">The r y.</param>
/// <param name="rZ">The r z.</param>
Camera::Camera(
	CameraOptions options,
	float pX, float pY, float pZ,
	float rX, float rY, float rZ
) : Camera(options, TransformDescription(pX, pY, pZ, rX, rY, rZ)) {}

/// <summary>
/// Initializes a new instance of the <see cref="Camera"/> class.
/// </summary>
/// <param name="options">The options.</param>
/// <param name="position">The position.</param>
/// <param name="rotation">The rotation.</param>
Camera::Camera(
	CameraOptions options,
	_In_reads_(3) const float* _position,
	_In_reads_(3) const float* _rotation
) : Camera(options, TransformDescription(_position, _rotation)) {}

/// <summary>
/// Finalizes an instance of the <see cref="Camera"/> class.
/// </summary>
Camera::~Camera() {}

/// <summary>
/// Initializes a new instance of the <see cref="Camera"/> class.
/// </summary>
/// <param name="other">The other.</param>
Camera::Camera(const Camera& other)
{
	// Copy member data.
	this->transform = other.transform; // We wrote our own copy assignment!
	this->settings = other.settings; // We wrote our own copy assignment!
	this->view = other.view;
	this->projection = other.projection;
	this->tracker = other.tracker;
}

/// <summary>
/// Initializes a new instance of the <see cref="Camera"/> class.
/// </summary>
/// <param name="other">The other.</param>
Camera::Camera(Camera&& other) noexcept
	: Camera() // Initialize the camera.
{
	swap(*this, other);
}

/// <summary>
/// Unified copy and move assignment operator.
/// </summary>
/// <param name="other">The other.</param>
/// <returns>Returns reference to the camera in question.</returns>
Camera& Camera::operator=(Camera other)
{
	swap(*this, other);
	return *this;
}

// ---------------------
// Accessors.

/// <summary>
/// Gets the view matrix.
/// </summary>
/// <returns>Returns a matrix.</returns>
XMFLOAT4X4 Camera::GetViewMatrix() const
{
	return this->view;
}

/// <summary>
/// Gets the view matrix.
/// </summary>
/// <param name="target">The target.</param>
void Camera::GetViewMatrix(XMFLOAT4X4& target) const
{
	target = this->view;
}

/// <summary>
/// Gets the projection matrix.
/// </summary>
/// <returns>Returns a matrix.</returns>
XMFLOAT4X4 Camera::GetProjectionMatrix() const
{
	return this->projection;
}

/// <summary>
/// Gets the projection matrix.
/// </summary>
/// <param name="target">The target.</param>
void Camera::GetProjectionMatrix(XMFLOAT4X4& target) const
{
	target = this->projection;
}

/// <summary>
/// Gets the transform.
/// </summary>
/// <returns>Returns the transform.</returns>
TransformDescription Camera::GetTransform() const
{
	return this->transform;
}

/// <summary>
/// Gets the transform.
/// </summary>
/// <param name="target">The target.</param>
void Camera::GetTransform(TransformDescription& target) const
{
	target = transform;
}

/// <summary>
/// Gets the mouse tracker.
/// </summary>
/// <returns>Returns the tracker.</returns>
const MouseTracker& Camera::GetMouseTracker() const
{
	return tracker;
}

/// <summary>
/// Gets the mouse tracker.
/// </summary>
/// <param name="target">The target.</param>
void Camera::GetMouseTracker(MouseTracker& target) const
{
	target = tracker;
}

// ---------------------
// Mutators.

/// <summary>
/// Sets the fov.
/// </summary>
/// <param name="fov">The fov.</param>
void Camera::SetFOV(float fov)
{
	this->settings.SetFieldOfView(fov);
}

/// <summary>
/// Sets the dimensions.
/// </summary>
/// <param name="width">The width.</param>
/// <param name="height">The height.</param>
void Camera::SetDimensions(float width, float height)
{
	this->settings.SetWidth(width);
	this->settings.SetHeight(height);
	this->OnResize();
}

/// <summary>
/// Sets the clipping plane.
/// </summary>
/// <param name="nearPlane">The near clipping plane distance.</param>
/// <param name="farPlane">The far clipping plane distance.</param>
void Camera::SetClippingPlane(float nearPlane, float farPlane)
{
	this->settings.SetNearClippingPlane(nearPlane);
	this->settings.SetFarClippingPlane(farPlane);
}

// ---------------------
// Service methods.

/// <summary>
/// Resets this instance.
/// </summary>
void Camera::Reset()
{
	this->transform.Reset();
	this->UpdateViewMatrix();
	this->UpdateProjectionMatrix();
}

/// <summary>
/// Updates the mouse.
/// </summary>
/// <param name="_currentState">if set to <c>true</c> [current state].</param>
/// <param name="_mouseX">The mouse x.</param>
/// <param name="_mouseY">The mouse y.</param>
void Camera::UpdateMouse(
	bool _currentState,
	float _mouseX, float _mouseY) 
{
	this->tracker.Update(_currentState, _mouseX, _mouseY);
}

/// <summary>
/// Updates the position.
/// </summary>
/// <param name="speed">The speed.</param>
/// <param name="isRelative">if set to <c>true</c> [is relative].</param>
void Camera::UpdatePosition(
	XMFLOAT3 speed, bool isRelative /* = true */)
{
	this->transform.UpdatePosition(speed, isRelative);
	this->UpdateViewMatrix();
}

/// <summary>
/// Updates the rotation.
/// </summary>
/// <param name="speed">The speed.</param>
/// <param name="isRelative">if set to <c>true</c> [is relative].</param>
void Camera::UpdateRotation(
	XMFLOAT3 speed, bool isRelative /* = true */)
{
	this->transform.UpdateRotation(speed, isRelative);
	this->UpdateViewMatrix();
}

/// <summary>
/// Updates the position.
/// </summary>
/// <param name="deltaTime">The delta time.</param>
/// <param name="totalTime">The total time.</param>
/// <param name="speed">The speed.</param>
/// <param name="isRelative">if set to <c>true</c> [is relative].</param>
void Camera::UpdatePosition(
	float deltaTime, float totalTime,
	XMFLOAT3 speed, bool isRelative /* = true */) 
{
	this->transform.UpdatePosition(deltaTime, totalTime, speed, isRelative);
	this->UpdateViewMatrix();
}

/// <summary>
/// Updates the rotation.
/// </summary>
/// <param name="deltaTime">The delta time.</param>
/// <param name="totalTime">The total time.</param>
/// <param name="speed">The speed.</param>
/// <param name="isRelative">if set to <c>true</c> [is relative].</param>
void Camera::UpdateRotation(
	float deltaTime, float totalTime,
	XMFLOAT3 speed, bool isRelative /* = true */) 
{
	this->transform.UpdateRotation(deltaTime, totalTime, speed, isRelative);
	this->UpdateViewMatrix();
}

/// <summary>
/// Updates the view matrix.
/// </summary>
void Camera::UpdateViewMatrix()
{
	this->view = CalculateViewMatrix();
}

/// <summary>
/// Updates the projection matrix.
/// </summary>
void Camera::UpdateProjectionMatrix()
{
	this->projection = CalculateProjectionMatrix();
}

/// <summary>
/// Calculates the view matrix.
/// </summary>
/// <param name="targert">The target.</param>
void Camera::CalculateViewMatrix(XMFLOAT4X4& target)
{
	target = this->view;
}

/// <summary>
/// Calculates the projection matrix.
/// </summary>
/// <param name="target">The target.</param>
void Camera::CalculateProjectionMatrix(XMFLOAT4X4& target)
{
	target = this->projection;
}

// ---------------------
// Private members.
// ---------------------

// ---------------------
// Accessors.

/// <summary>
/// Gets the current position.
/// </summary>
/// <returns>Returns float vector.</returns>
XMFLOAT3 Camera::GetCurrentPosition() const
{
	return this->transform.GetCurrentPosition();
}

/// <summary>
/// Gets the current position.
/// </summary>
/// <param name="target">The target.</param>
void Camera::GetCurrentPosition(XMFLOAT3& target) const
{
	this->transform.GetCurrentPosition(target);
}

/// <summary>
/// Gets the current orientation.
/// </summary>
/// <returns>Returns float vector.</returns>
XMFLOAT3 Camera::GetCurrentOrientation() const 
{
	return this->transform.GetCurrentOrientation();
}

/// <summary>
/// Gets the current orientation.
/// </summary>
/// <param name="target">The target.</param>
void Camera::GetCurrentOrientation(XMFLOAT3& target) const
{
	this->transform.GetCurrentOrientation(target);
}

/// <summary>
/// Gets the current orientation.
/// </summary>
/// <returns>Returns float vector.</returns>
CameraOptions Camera::GetSettings() const 
{
	return this->settings;
}


// ---------------------
// Mutators.

/// <summary>
/// Moves camera by delta position.
/// </summary>
/// <param name="delta">The delta.</param>
void Camera::MoveBy(XMFLOAT3 delta)
{
	this->transform.Translate(delta);
	this->UpdateViewMatrix();
}

/// <summary>
/// Sets camera position.
/// </summary>
/// <param name="absolute">The vector.</param>
void Camera::MoveTo(XMFLOAT3 absolute) 
{
	this->transform.SetPosition(absolute);
	this->UpdateViewMatrix();
}

/// <summary>
/// Rotates camera by delta position.
/// </summary>
/// <param name="delta">The delta.</param>
void Camera::RotateBy(XMFLOAT3 delta) 
{
	this->transform.Rotate(delta);
	this->UpdateViewMatrix();
}

/// <summary>
/// Sets camera orientation.
/// </summary>
/// <param name="absolute">The vector.</param>
void Camera::RotateTo(XMFLOAT3 absolute)
{
	this->transform.SetRotation(absolute);
	this->UpdateViewMatrix();
}

// ---------------------
// Helper methods.

/// <summary>
/// Called when [resize].
/// </summary>
void Camera::OnResize()
{
	this->settings.UpdateAspectRatio();
	this->UpdateProjectionMatrix();
}

/// <summary>
/// Calculates the view matrix.
/// </summary>
/// <returns>Returns the matrix in XMFLOAT4X4 format.</returns>
XMFLOAT4X4 Camera::CalculateViewMatrix()
{
	// Get view matrix data.
	XMFLOAT3 p = this->GetCurrentPosition();
	XMFLOAT3 d = this->transform.GetHeading();
	XMFLOAT3 u = this->transform.GetUp();

	// Convert the values.
	XMVECTOR POS = XMVectorSet(p.x, p.y, p.z, 0.0f);
	XMVECTOR DIR = XMVectorSet(d.x, d.y, d.z, 0.0f);
	XMVECTOR UP = XMVectorSet(u.x, u.y, u.z, 0.0f);

	// Calculate the matrix.
	XMMATRIX VIEW = XMMatrixLookToLH(
		POS, // The position of the camera.
		DIR, // Direction the camera is looking.
		UP // "Up" direction in 3D space (prevents roll).
	);

	// Store in safe format.
	XMFLOAT4X4 v;
	XMStoreFloat4x4(&v, XMMatrixTranspose(VIEW));

	// Return the matrix.
	return v;
}

/// <summary>
/// Calculates the projection matrix.
/// </summary>
/// <returns>Returns the matrix in XMFLOAT4X4 format.</returns>
XMFLOAT4X4 Camera::CalculateProjectionMatrix()
{
	// Projection matrix values.
	float fov = this->settings.GetFieldOfView();
	float aspectRatio = this->settings.GetAspectRatio();
	float nearPlane = this->settings.GetNearClippingPlane();
	float farPlane = this->settings.GetFarClippingPlane();

	// Calculation of the projection matrix with most recent changes.
	XMMATRIX PROJ = XMMatrixPerspectiveFovLH(
		fov,
		aspectRatio,
		nearPlane,
		farPlane
	);

	// Store as transposed matrix.
	XMFLOAT4X4 proj;
	XMStoreFloat4x4(&proj, XMMatrixTranspose(PROJ));

	// Return the matrix.
	return proj;
}

#pragma endregion