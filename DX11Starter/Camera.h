#pragma once

// --------------------------------------
// Include statements.
// --------------------------------------
#include <DirectXMath.h>
#include <array>

// ------------------------------------
// Aiding structures.
// ------------------------------------

const float PI = 3.1415926535f; // Rounded PI value.

/// <summary>
/// Contains options for Camera objects.
/// </summary>
struct CameraOptions {

public:

	// --------------------
	// Static methods.

	static CameraOptions GetDefaultCameraOptions();

	// --------------------
	// Friend methods.

	friend void swap(CameraOptions& lhs, CameraOptions& rhs);

	// --------------------
	// Constructor(s).

	CameraOptions();
	CameraOptions(
		float fov, float w, float h,
		float nearClip, float farClip
	);
	~CameraOptions();
	CameraOptions(const CameraOptions& other);
	CameraOptions(CameraOptions&& other) noexcept;
	CameraOptions& operator=(CameraOptions other);

	// --------------------
	// Accessors.

	float GetFieldOfView() const;
	float GetWidth() const;
	float GetHeight() const;
	float GetAspectRatio() const;
	float GetNearClippingPlane() const;
	float GetFarClippingPlane() const;

	// --------------------
	// Mutators.

	void SetFieldOfView(float value);
	void SetWidth(float value);
	void SetWidthByAspectRatio(float ratio, float height);
	void SetHeight(float value);
	void SetHeightByAspectRatio(float ratio, float width);
	void SetNearClippingPlane(float value);
	void SetFarClippingPlane(float value);

	// --------------------
	// Service methods.

	void UpdateAspectRatio();

private:

	// --------------------
	// Data members.

	float fieldOfView; // Field of View angle.
	float width; // Viewport width.
	float height; // Viewport height.
	float aspectRatio; // Update the calculated aspect ratio.
	float nearPlane; // Near clipping plane.
	float farPlane; // Far clipping plane.

};

/// <summary>
/// Holds normalized unit vectors.
/// </summary>
struct UnitVector {

	// --------------------
	// Static Methods.

	static UnitVector GetDefaultUp();
	static UnitVector GetDefaultRight();
	static UnitVector GetDefaultForward();

	// --------------------
	// Friend methods.

	// Reference: https://stackoverflow.com/questions/3279543/what-is-the-copy-and-swap-idiom
	friend void swap(UnitVector& lhs, UnitVector& rhs);

	// -------------
	// Constructor(s).

	UnitVector();
	UnitVector(DirectX::XMFLOAT3 vector);
	UnitVector(float x, float y, float z);
	UnitVector(_In_reads_(3) const float* source);
	~UnitVector(); // Destructor.
	UnitVector(const UnitVector& other); // Copy constructor.
	UnitVector(UnitVector&& other) noexcept; // Move constructor.
	UnitVector& operator=(UnitVector other); // Move/Copy assignment. Both handled via C++11.

	// -------------
	// Accessors.

	DirectX::XMFLOAT3 Get() const;
	float Get(unsigned int dimension) const;

	DirectX::XMFLOAT3 Scale(float magnitude) const;

	// -------------
	// Mutators.

	void Set(float x, float y, float z);
	void Set(_In_reads_(3) const float* source);
	void Set(DirectX::XMFLOAT3 vector);

private:

	// -------------
	// Data members.

	std::array<float, 3> data;

	// -------------
	// Helper methods.

	void normalize();

};

/// <summary>
/// Wrapper for functionality to handle movement.
/// </summary>
struct TransformDescription {

	// --------------------
	// Static Methods.

	static TransformDescription GetDefaultTransform();

	// --------------------
	// Friend methods.

	friend void swap(TransformDescription& lhs, TransformDescription& rhs);

	// -------------
	// Constructor.

	TransformDescription();
	TransformDescription(DirectX::XMFLOAT3 _position);
	TransformDescription(
		DirectX::XMFLOAT3 _position,
		DirectX::XMFLOAT3 _rotation
	);
	TransformDescription(
		float pX, float pY, float pZ,
		float rX, float rY, float rZ
	);
	TransformDescription(
		_In_reads_(3) const float* _position,
		_In_reads_(3) const float* _rotation
	);
	~TransformDescription();
	TransformDescription(const TransformDescription& other);
	TransformDescription(TransformDescription&& other) noexcept;
	TransformDescription& operator=(TransformDescription other);

	// -------------
	// Accessors.

	DirectX::XMFLOAT3 GetStartingPosition() const;
	void GetStartingPosition(DirectX::XMFLOAT3& target) const;

	DirectX::XMFLOAT3 GetCurrentPosition() const;
	void GetCurrentPosition(DirectX::XMFLOAT3& target) const;

	DirectX::XMFLOAT3 GetBaseOrientation() const;
	void GetBaseOrientation(DirectX::XMFLOAT3& target) const;

	DirectX::XMFLOAT3 GetCurrentOrientation() const;
	void GetCurrentOrientation(DirectX::XMFLOAT3& target) const;

	DirectX::XMFLOAT3 GetUp() const;
	void GetUp(DirectX::XMFLOAT3& target) const;

	DirectX::XMFLOAT3 GetHeading() const;
	void GetHeading(DirectX::XMFLOAT3& target) const;

	// -------------
	// Mutators.

	void Translate(UnitVector direction, float scale);
	void Translate(DirectX::XMFLOAT3 delta);
	void Rotate(DirectX::XMFLOAT3 delta);

	void SetPosition(DirectX::XMFLOAT3 absolute);
	void SetRotation(DirectX::XMFLOAT3 absolute);

	// -------------
	// Service methods.

	void Reset();
	void UpdatePosition(DirectX::XMFLOAT3 speed, bool isRelative = true); // Relative translation.
	void UpdateRotation(DirectX::XMFLOAT3 speed, bool isRelative = true); // Relative rotation.
	void UpdatePosition(float deltaTime, float totalTime, DirectX::XMFLOAT3 speed, bool isRelative = true); // Relative translation.
	void UpdateRotation(float deltaTime, float totalTime, DirectX::XMFLOAT3 speed, bool isRelative = true); // Relative rotation.

private:

	// -------------
	// Data members.

	UnitVector up; // Up vector.
	UnitVector heading; // Forward vector.
	DirectX::XMFLOAT3 position_start; // Starting position in local space.
	DirectX::XMFLOAT3 position; // Position of user camera in local space.
	DirectX::XMFLOAT3 orientation_start; // Yaw (x), Pitch (y), Roll (z) for base orientation.
	DirectX::XMFLOAT3 orientation; // Yaw (x), Pitch (y), Roll (z) for camera rotation. Rotation around z-axis is usually locked.

	// -------------
	// Helper members.

	void CalculateHeading();
	void CalculateUp();

};

/// <summary>
/// Wrapper for tracking mouse state.
/// </summary>
struct MouseTracker {
public:
	
	// ------------------------------------
	// Friend methods.
	// ------------------------------------

	friend void swap(MouseTracker& lhs, MouseTracker& rhs);
	
	template <typename T>
	friend T clamp(const T& n, const T& low, const T& high);

	template <typename T>
	friend T range(const T& n, const T& originalLow, const T& originalHigh, const T& newLow, const T& newHigh);

	// ------------------------------------
	// Constructor(s).
	// ------------------------------------

	MouseTracker();
	MouseTracker(const MouseTracker& other);
	MouseTracker(MouseTracker&& other) noexcept;
	MouseTracker& operator=(MouseTracker other);

	// ------------------------------------
	// Accessors.
	// ------------------------------------

	bool GetPreviousButtonState() const;
	const DirectX::XMFLOAT2& GetPreviousMousePosition() const;

	bool GetCurrentButtonState() const;
	const DirectX::XMFLOAT2& GetCurrentMousePosition() const;

	DirectX::XMFLOAT2 GetRawDelta() const;
	float GetRawHorizontalDelta() const;
	float GetRawVerticalDelta() const;

	DirectX::XMFLOAT2 GetDelta(DirectX::XMFLOAT2 _min, DirectX::XMFLOAT2 _max) const;
	float GetHorizontalDelta(float _min, float _max) const;
	float GetVerticalDelta(float _min, float _max) const;

	// ------------------------------------
	// Service methods.
	// ------------------------------------

	void Update(bool _currentState, DirectX::XMFLOAT2 _currentPosition);
	void Update(bool _currentState, float _mouseX, float _mouseY);

private:

	// ------------------------------------
	// Data members.
	// ------------------------------------

	bool previousButtonState;
	bool currentButtonState;
	DirectX::XMFLOAT2 previousMousePosition;
	DirectX::XMFLOAT2 currentMousePosition;

	// ------------------------------------
	// Accessors.
	// ------------------------------------


	// ------------------------------------
	// Mutators.
	// ------------------------------------

	void SetPreviousButtonState(bool _state);
	void SetPreviousMousePosition(DirectX::XMFLOAT2 _position);

	void SetCurrentButtonState(bool _state);
	void SetCurrentMousePosition(DirectX::XMFLOAT2 _position);

	// ------------------------------------
	// Helper methods.
	// ------------------------------------

	DirectX::XMFLOAT2 CalculateDelta() const;
	float CalculateDeltaHorizontal() const;
	float CalculateDeltaVertical() const;
};

// ------------------------------------
// ------------------------------------
// Main class.
// ------------------------------------
// ------------------------------------

/// <summary>
/// Defines a view in a 3D scene.
/// </summary>
class Camera
{
public:
	// ------------------------------------
	// Static methods.
	// ------------------------------------

	static Camera GetDefaultCamera();

	// ------------------------------------
	// Friend methods.
	// ------------------------------------

	friend void swap(Camera& lhs, Camera& rhs);

	// ------------------------------------
	// Constructors.
	// ------------------------------------

	Camera();
	Camera(CameraOptions options);
	Camera(CameraOptions options, TransformDescription transform);
	Camera(CameraOptions options, DirectX::XMFLOAT3 position);
	Camera(
		CameraOptions options,
		DirectX::XMFLOAT3 position,
		DirectX::XMFLOAT3 rotation
	);
	Camera(
		CameraOptions options,
		float pX, float pY, float pZ,
		float rX, float rY, float rZ
	);
	Camera(
		CameraOptions options,
		_In_reads_(3) const float* _position,
		_In_reads_(3) const float* _rotation
	);
	~Camera();
	Camera(const Camera& other);
	Camera(Camera&& other) noexcept;
	Camera& operator=(Camera other);

	// ------------------------------------
	// Accessors.
	// ------------------------------------

	DirectX::XMFLOAT4X4 GetViewMatrix() const;
	void GetViewMatrix(DirectX::XMFLOAT4X4& target) const;

	DirectX::XMFLOAT4X4 GetProjectionMatrix() const;
	void GetProjectionMatrix(DirectX::XMFLOAT4X4& target) const;

	TransformDescription GetTransform() const;
	void GetTransform(TransformDescription& target) const;

	const MouseTracker& GetMouseTracker() const;
	void GetMouseTracker(MouseTracker& target) const;

	// ------------------------------------
	// Mutators.
	// ------------------------------------

	void SetFOV(float fov);
	void SetDimensions(float width, float height);
	void SetClippingPlane(float nearPlane, float farPlane);

	// ------------------------------------
	// Service methods.
	// ------------------------------------

	void Reset();
	void UpdateMouse(bool _currentState, float _mouseX, float _mouseY);
	void UpdatePosition(DirectX::XMFLOAT3 speed, bool isRelative = true); // Relative translation.
	void UpdateRotation(DirectX::XMFLOAT3 speed, bool isRelative = true); // Relative rotation.
	void UpdatePosition(float deltaTime, float totalTime, DirectX::XMFLOAT3 speed, bool isRelative = true); // Relative translation.
	void UpdateRotation(float deltaTime, float totalTime, DirectX::XMFLOAT3 speed, bool isRelative = true); // Relative rotation.

	void UpdateViewMatrix();
	void UpdateProjectionMatrix();
	
	void CalculateViewMatrix(DirectX::XMFLOAT4X4& target);
	void CalculateProjectionMatrix(DirectX::XMFLOAT4X4& target);

private:

	// ------------------------------------
	// Data members.
	// ------------------------------------

	MouseTracker tracker; // Tracks mouse movement.
	TransformDescription transform; // Contains position/orientation information.
	CameraOptions settings; // Contains the settings for the camera object.
	DirectX::XMFLOAT4X4 view; // Stores the view matrix.
	DirectX::XMFLOAT4X4 projection; // Stores the projection matrix.

	// ------------------------------------
	// Accessors.
	// ------------------------------------

	DirectX::XMFLOAT3 GetCurrentPosition() const;
	void GetCurrentPosition(DirectX::XMFLOAT3& target) const;

	DirectX::XMFLOAT3 GetCurrentOrientation() const;
	void GetCurrentOrientation(DirectX::XMFLOAT3& target) const;

	CameraOptions GetSettings() const;

	// ------------------------------------
	// Mutators.
	// ------------------------------------

	void MoveBy(DirectX::XMFLOAT3 delta);
	void MoveTo(DirectX::XMFLOAT3 absolute);

	void RotateBy(DirectX::XMFLOAT3 delta);
	void RotateTo(DirectX::XMFLOAT3 absolute);

	// ------------------------------------
	// Helper methods.
	// ------------------------------------

	void OnResize(); // Update the projection matrix.

	DirectX::XMFLOAT4X4 CalculateViewMatrix();

	DirectX::XMFLOAT4X4 CalculateProjectionMatrix();

};
