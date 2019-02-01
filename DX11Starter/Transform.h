#pragma once

// -----------------------------------------------
// Include statements.
// -----------------------------------------------
#include <DirectXMath.h>

// -----------------------------------------------
// Transform.h | Ian Effendi (effendiian@gmail.com)
// ---
// Wrapper class encapsulates transformation
// calculations of world matrix vectors.
// -----------------------------------------------

// --------------------------------------------------------
// A custom transform definition
// --------------------------------------------------------
struct TRANSFORM
{
public:

	// ----------------------------------------------------
	// Modelled after union in XMFLOAT4X4 declaration.
	// ----------------------------------------------------

	union {
		struct {
			float pX, pY, pZ;
			float sX, sY, sZ;
			float rX, rY, rZ, rW;
		};
		float m[10];
	};

	// ----------------------------------------------------
	// Constructors.
	// ----------------------------------------------------
	TRANSFORM() = default;

	// ----------------------------------------------------
	// Move and Copy assignment operator overloads.
	// ----------------------------------------------------
	TRANSFORM(const TRANSFORM&) = default;
	TRANSFORM& operator=(const TRANSFORM&) = default;

	TRANSFORM(TRANSFORM&&) = default;
	TRANSFORM& operator=(TRANSFORM&&) = default;

	// ----------------------------------------------------
	// Compile-time expression for initializing this struct.
	// ----------------------------------------------------
	constexpr TRANSFORM(
		float _posX, float _posY, float _posZ,
		float _scaleX, float _scaleY, float _scaleZ,
		float _rotX, float _rotY, float _rotZ, float _rotW
	) : pX(_posX), pY(_posY), pZ(_posZ),
		sX(_posX), sY(_posY), sZ(_posZ),
		rX(_rotX), rY(_rotY), rZ(_rotZ), rW(_rotW) {};

	// ----------------------------------------------------
	// Explicit constructor.
	// Can assign transform with an ordered array of floats.
	// ----------------------------------------------------

	/// <summary>
	/// Initializes a new instance of the TRANSFORM structure from a ten element float array. Should be row-major.
	/// </summary>
	/// <param name="pArray">Array of values.</param>
	explicit TRANSFORM(_In_reads_(10) const float *pArray)
		: pX(pArray[0]), pY(pArray[1]), pZ(pArray[2]),
		sX(pArray[3]), sY(pArray[4]), sZ(pArray[5]),
		rX(pArray[6]), rY(pArray[7]), rZ(pArray[8]), rW(pArray[9]) {
		/* if (pArray) {
			pX = pArray[0]; pY = pArray[1]; pZ = pArray[2];
			sX = pArray[3];	sY = pArray[4];	sZ = pArray[5];
			rX = pArray[6];	rY = pArray[7];	rZ = pArray[8];	rW = pArray[9];
		} */
	};

	// ----------------------------------------------------
	// Static methods.
	// ----------------------------------------------------

	// ----------
	// POSITION
	static void StorePosition(TRANSFORM& target, const float _x, const float _y, const float _z);
	static void StorePosition(TRANSFORM& target, _In_reads_(3) const float *sourceArray);
	static void StorePosition(TRANSFORM& target, const DirectX::XMFLOAT3&  source);
	static void LoadPosition(DirectX::XMFLOAT3& target, const TRANSFORM& source);


	// ----------
	// SCALE
	static void StoreScale(TRANSFORM& target, const float _x, const float _y, const float _z);
	static void StoreScale(TRANSFORM& target, _In_reads_(3) const float *sourceArray);
	static void StoreScale(TRANSFORM& target, const DirectX::XMFLOAT3& source);
	static void LoadScale(DirectX::XMFLOAT3& target, const TRANSFORM& source);

	// ----------
	// ROTATION
	static void StoreRotation(TRANSFORM& target, const float _x, const float _y, const float _z, const float _w);
	static void StoreRotation(TRANSFORM& target, _In_reads_(4) const float *sourceArray);
	static void StoreRotation(TRANSFORM& target, const DirectX::XMFLOAT4& source);
	static void LoadRotation(DirectX::XMFLOAT4& target, const TRANSFORM& source);

	// ----------
	// WORLD MATRIX
	static void LoadWorldMatrix(DirectX::XMFLOAT4X4& target, const TRANSFORM& source);


	// ----------------------------------------------------
	// Mutators.
	// ----------------------------------------------------

	// ----------
	// POSITION
	void SetPosition(float x, float y, float z);
	void SetPosition(_In_reads_(3) const float *sourceArray);
	void SetPosition(const DirectX::XMFLOAT3& position);

	// ----------
	// SCALE
	void SetScale(float x, float y, float z);
	void SetScale(_In_reads_(3) const float *sourceArray);
	void SetScale(const DirectX::XMFLOAT3& scale);

	// ----------
	// ROTATION
	void SetRotation(float x, float y, float z, float w);
	void SetRotation(_In_reads_(4) const float *sourceArray);
	void SetRotation(const DirectX::XMFLOAT4& rotationQuaternion);

	// ----------------------------------------------------
	// Accessors.
	// ----------------------------------------------------

	// Note: On returning a reference or an object
	// Harold Serrano: https://www.haroldserrano.com/blog/c-tip-14-dont-return-a-reference-when-you-must-return-an-object

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

	// ----------------------------------------------------
	// Service methods.
	// ----------------------------------------------------

	// ----------
	// ROTATION
	void SetQuaternionRotationRollPitchYaw(float pitchY, float yawX, float rollZ);
	void SetQuaternionRotationRollPitchYaw(_In_reads_(3) const float *sourceArray);

	// ----------
	// WORLD MATRIX
	void CalculateWorldMatrix(DirectX::XMFLOAT4X4& target) const;

};