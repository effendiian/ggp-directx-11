#pragma once

// --------------------------------------
// Include statements.
// --------------------------------------
#include <DirectXMath.h>

/// <summary>
/// Defines a view in a 3D scene.
/// </summary>
class Camera
{
public:

	// ------------------------------------
	// Internal Struct.
	// ------------------------------------
	
	/// <summary>
	/// Holds normalized unit vectors.
	/// </summary>
	struct UnitVector {

		// --------------------
		// Static Methods.

		static UnitVector GetUp();
		static UnitVector GetRight();
		static UnitVector GetForward();

		// -------------
		// Constructor.

		UnitVector(DirectX::XMFLOAT3 vector);
		UnitVector(float x, float y, float z);
		UnitVector(float* data);

		// -------------
		// Accessors.

		DirectX::XMFLOAT3 Get() const;
		float Get(unsigned int dimension) const;

		DirectX::XMFLOAT3 Scale(float magnitude) const;

		// -------------
		// Mutators.

		void Set(DirectX::XMFLOAT3 vector);
		void Set(float x, float y, float z);
		void Set(float* data);
		
	private:

		// -------------
		// Data members.

		float* internal_array;
		
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
		
		// -------------
		// Constructor.

		TransformDescription();
		TransformDescription(DirectX::XMFLOAT3 position);
		TransformDescription(
			DirectX::XMFLOAT3 position,
			DirectX::XMFLOAT3 orientation,
			DirectX::XMFLOAT3 rotation
		);
		TransformDescription(
			float pX, float pY, float pZ,
			float oX, float oY, float oZ,
			float rX, float rY, float rZ
		);
		TransformDescription(
			float* position, 
			float* orientation, 
			float* rotation
		);

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

		UnitVector GetHeading() const;

		// -------------
		// Service methods.
		void Update(float deltaTime, float totalTime);
		void UpdatePosition(float deltaTime, float totalTime, DirectX::XMFLOAT3 speed, bool isRelative = true); // Relative translation.
		void UpdateRotation(float deltaTime, float totalTime, DirectX::XMFLOAT3 rotation, bool isRelative = true); // Relative rotation.

	private:

		// -------------
		// Data members.

		DirectX::XMFLOAT3 position_start; // Starting position in local space.
		DirectX::XMFLOAT3 position; // Position of user camera in local space.
		DirectX::XMFLOAT3 orientation_start; // Yaw (x), Pitch (y), Roll (z) for base orientation.
		DirectX::XMFLOAT3 orientation; // Yaw (x), Pitch (y), Roll (z) for camera rotation. Rotation around z-axis is usually locked.

		// -------------
		// Mutators.

		void Translate(DirectX::XMFLOAT3 delta);
		void Rotate(DirectX::XMFLOAT3 delta);
		void SetPosition(DirectX::XMFLOAT3 absolute);
		void SetRotation(DirectX::XMFLOAT3 absolute);

	};
	
	/// <summary>
	/// Contains options for Camera objects.
	/// </summary>
	struct CameraOptions {

	public:

		// --------------------
		// Static methods.
				
		static CameraOptions GetDefaultCameraOptions();
		const float PI = 3.1415926535f; // Rounded PI value.

		// --------------------
		// Constructor(s).

		CameraOptions();
		CameraOptions(
			float fov, float w, float h,
			float nearClip, float farClip
		);

		// --------------------
		// Accessors.

		float GetFieldOfView() const;
		float GetWidth() const;
		float GetHeight() const;
		float GetNearClippingPlane() const;
		float GetFarClippingPlane() const;
		float GetAspectRatio() const;
		DirectX::XMFLOAT4X4 GetAspectRatio() const;

		// --------------------
		// Mutators.

		void SetFieldOfView(float value);
		void SetWidth(float value);
		void SetHeight(float value);
		void SetNearClippingPlane(float value);
		void SetFarClippingPlane(float value);
		void SetAspectRatio(float value);
		
		// --------------------
		// Service methods.

		void CalculateAspectRatio();

	private:

		// --------------------
		// Data members.

		float fieldOfView; // Field of View angle.
		float width; // Viewport width.
		float height; // Viewport height.
		struct planes {
			float near; // Near clipping plane.
			float far; // Far clipping plane.
		};

		// --------------------
		// Accessors.
		

		// --------------------
		// Mutators.

		// --------------------
		// Helper methods.


	};

	// ------------------------------------
	// Constructors.
	// ------------------------------------

	Camera();
	Camera(TransformDescription transform);
	Camera(DirectX::XMFLOAT3 position);
	Camera(
		DirectX::XMFLOAT3 position,
		DirectX::XMFLOAT3 orientation,
		DirectX::XMFLOAT3 rotation
	);
	Camera(
		float pX, float pY, float pZ,
		float oX, float oY, float oZ,
		float rX, float rY, float rZ
	);
	Camera(
		float* position,
		float* orientation,
		float* rotation
	);
	
	// ------------------------------------
	// Accessors.
	// ------------------------------------

	DirectX::XMFLOAT4X4 GetViewMatrix() const;
	void GetViewMatrix(DirectX::XMFLOAT4X4& target) const;

	DirectX::XMFLOAT4X4 GetProjectionMatrix() const;
	void GetProjectionMatrix(DirectX::XMFLOAT4X4& target) const;

	TransformDescription GetTransform() const;
	void GetTransform(TransformDescription& target) const;

	// ------------------------------------
	// Mutators.
	// ------------------------------------

	void MoveBy(DirectX::XMFLOAT3 delta);
	void MoveTo(DirectX::XMFLOAT3 absolute);

	void RotateBy(DirectX::XMFLOAT3 delta);
	void RotateTo(DirectX::XMFLOAT3 absolute);

	// ------------------------------------
	// Service methods.
	// ------------------------------------

private:

	// ------------------------------------
	// Data members.
	// ------------------------------------

	TransformDescription transform; // Contains position/orientation information.
	DirectX::XMFLOAT4X4 view; // Stores the view matrix.
	DirectX::XMFLOAT4X4 projection; // Stores the projection matrix.

	// ------------------------------------
	// Accessors.
	// ------------------------------------

	DirectX::XMFLOAT3 GetCurrentPosition() const;
	void GetCurrentPosition(DirectX::XMFLOAT3& target) const;

	DirectX::XMFLOAT3 GetCurrentOrientation() const;
	void GetCurrentOrientation(DirectX::XMFLOAT3& target) const;

	// ------------------------------------
	// Helper methods.
	// ------------------------------------

	void CalculateViewMatrix(DirectX::XMFLOAT4X4 source);
	void CalculateProjectionMatrix(DirectX::XMFLOAT4X4 source);

};

