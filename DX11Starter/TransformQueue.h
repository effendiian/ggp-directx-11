#pragma once

// -----------------------------------------------
// Include statements.
// -----------------------------------------------
#include <memory>
#include <queue>
#include <DirectXMath.h>
#include "Transform.h"

class TransformQueue
{
public:
	// -----------------------------------------------
	// Internal enum/structs.
	// -----------------------------------------------

	/// <summary>
	/// TRANSFORM_TYPE determines the transformation type
	/// that is being stored with a particular XMFLOAT3
	/// or XMFLOAT4 value.
	/// </summary>
	typedef enum _TRANSFORM_TYPE
	{
		T_NONE = 0,
		T_POSITION = 1,
		T_SCALE = 2,
		T_ROTATION = 3
	} TRANSFORM_TYPE;

	// -----------------------------------------------
	// Constructors.
	// -----------------------------------------------

	// Creates an empty queue.
	TransformQueue();

	// Destructor.
	~TransformQueue();

	// Copy constructor.
	TransformQueue(const TransformQueue& other);

	// Copy assignment operator.
	TransformQueue& operator=(TransformQueue other);

	// -----------------------------------------------
	// Static methods.
	// -----------------------------------------------

	// Check if a transformation is of a particular type.
	static bool IsPosition(std::pair<TRANSFORM_TYPE, DirectX::XMFLOAT4>& transformation);
	static bool IsScale(std::pair<TRANSFORM_TYPE, DirectX::XMFLOAT4>& transformation);
	static bool IsRotation(std::pair<TRANSFORM_TYPE, DirectX::XMFLOAT4>& transformation);

	// Convert float vector values.
	static void ConvertTransformation(DirectX::XMFLOAT3& target, const DirectX::XMFLOAT4& source);
	static void ConvertTransformation(DirectX::XMFLOAT4& target, const DirectX::XMFLOAT3& source);

	// -----------------------------------------------
	// Accessors.
	// -----------------------------------------------

	// ------------
	// POSITION
	const DirectX::XMFLOAT3 peek_position() const;
	void pop_position(DirectX::XMFLOAT3& target);
	void pop_position(DirectX::XMFLOAT4& target);

	// ------------
	// SCALE
	const DirectX::XMFLOAT3 peek_scale() const;
	void pop_scale(DirectX::XMFLOAT3& target);
	void pop_scale(DirectX::XMFLOAT4& target);

	// ------------
	// ROTATION
	const DirectX::XMFLOAT4 peek_rotation() const;
	void pop_rotation(DirectX::XMFLOAT4& target);


	// -----------------------------------------------
	// Mutators.
	// -----------------------------------------------

	// ------------
	// POSITION
	void push_position(float x, float y, float z);
	void push_position(_In_reads_(3) const float *pArray);
	void push_position(const DirectX::XMFLOAT3& source);
	void push_position(const DirectX::XMFLOAT4& source);
	void push_position(const TRANSFORM& source);

	// ------------
	// SCALE
	void push_scale(float x, float y, float z);
	void push_scale(_In_reads_(3) const float *pArray);
	void push_scale(const DirectX::XMFLOAT3& source);
	void push_scale(const DirectX::XMFLOAT4& source);
	void push_scale(const TRANSFORM& source);

	// ------------
	// ROTATION
	void push_rotation(float x, float y, float z, float w);
	void push_rotation(_In_reads_(4) const float *pArray);
	void push_rotation(const DirectX::XMFLOAT4& source);
	void push_rotation(const TRANSFORM& source);


	// -----------------------------------------------
	// Service methods.
	// -----------------------------------------------

	// Clears the queue of all transforms.
	void clear();

private:

	// -----------------------------------------------
	// Data members.
	// -----------------------------------------------

	// Stores position, scale, or rotation transformations.
	std::queue<std::pair<TRANSFORM_TYPE, DirectX::XMFLOAT4>> transformQueue;

	// -----------------------------------------------
	// Helper methods.
	// -----------------------------------------------

	// ------------
	// PAIR BUILDERS
	void pair_transformation(std::pair<TRANSFORM_TYPE, DirectX::XMFLOAT4>& target, TRANSFORM_TYPE type, const DirectX::XMFLOAT4& transformation);
	void pair_transformation(std::pair<TRANSFORM_TYPE, DirectX::XMFLOAT4>& target, TRANSFORM_TYPE type, const DirectX::XMFLOAT3& transformation);
	void pair_position(std::pair<TRANSFORM_TYPE, DirectX::XMFLOAT4>& target, const DirectX::XMFLOAT3 transformation);
	void pair_position(std::pair<TRANSFORM_TYPE, DirectX::XMFLOAT4>& target, const DirectX::XMFLOAT4 transformation);
	void pair_scale(std::pair<TRANSFORM_TYPE, DirectX::XMFLOAT4>& target, const DirectX::XMFLOAT3 transformation);
	void pair_scale(std::pair<TRANSFORM_TYPE, DirectX::XMFLOAT4>& target, const DirectX::XMFLOAT4 transformation);
	void pair_rotation(std::pair<TRANSFORM_TYPE, DirectX::XMFLOAT4>& target, const DirectX::XMFLOAT4 transformation);

	// ------------
	// TRANSFORM TYPE COMPARISONS
	bool is_type(TRANSFORM_TYPE a, TRANSFORM_TYPE b) const;
	bool is_type(TRANSFORM_TYPE a, std::pair<TRANSFORM_TYPE, DirectX::XMFLOAT4>& pair) const;
};

