#pragma once

// -----------------------------------------------
// Include statements.
// -----------------------------------------------
#include <memory>
#include <queue>
#include <DirectXMath.h>
#include "Transform.h"

class TransformBuffer
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

	/// <summary>
	/// Wrapper for a pair between a Transform type and an XMFLOAT4 type.
	/// </summary>
	typedef std::pair<TRANSFORM_TYPE, DirectX::XMFLOAT4> TransformPair;

	/// <summary>
	/// Wrapper for a queue of transformations.
	/// </summary>
	typedef std::queue<TransformPair> TransformQueue;

	// -----------------------------------------------
	// Constructors.
	// -----------------------------------------------

	// Creates an empty queue.
	TransformBuffer();

	// -----------------------------------------------
	// Static methods.
	// -----------------------------------------------

	// Check if a transformation is of a particular type.
	static bool TransformBuffer::IsMatchingTransformType(TRANSFORM_TYPE a, TRANSFORM_TYPE b);
	static bool IsOfTransformType(TRANSFORM_TYPE type, const TransformPair& transformation);
	static bool IsNull(const TransformPair& transformation);
	static bool IsPosition(const TransformPair& transformation);
	static bool IsScale(const TransformPair& transformation);
	static bool IsRotation(const TransformPair& transformation);

	// Convert float vector values.
	static void ConvertTransformation(DirectX::XMFLOAT3& target, const DirectX::XMFLOAT4& source);
	static void ConvertTransformation(DirectX::XMFLOAT4& target, const DirectX::XMFLOAT3& source, float lastValue = 0.0f);

	// -----------------------------------------------
	// Accessors.
	// -----------------------------------------------

	// ------------
	// TRANSFORMPAIR
	const TransformPair peek_pair() const;
	const DirectX::XMFLOAT4 peek_float4() const;
	const DirectX::XMFLOAT3 peek_float3() const;
	void peek(TransformPair& target) const;
	void peek(DirectX::XMFLOAT4& target) const;
	void peek(DirectX::XMFLOAT3& target) const;

	// -----------------------------------------------
	// Mutators.
	// -----------------------------------------------

	// ------------
	// TRANSFORMPAIR
	void pop();

	// ------------
	// POSITION
	void push_position(const DirectX::XMFLOAT4& source);
	void push_position(const DirectX::XMFLOAT3& source);
	void push_position(const TRANSFORM& source);
	void push_position(float x, float y, float z);
	void push_position(_In_reads_(3) const float *data);

	// ------------
	// SCALE
	void push_scale(const DirectX::XMFLOAT4& source);
	void push_scale(const DirectX::XMFLOAT3& source);
	void push_scale(const TRANSFORM& source);
	void push_scale(float x, float y, float z);
	void push_scale(_In_reads_(3) const float *data);

	// ------------
	// ROTATION
	void push_rotation(const DirectX::XMFLOAT4& source);
	void push_rotation(const TRANSFORM& source);
	void push_rotation(float x, float y, float z, float w);
	void push_rotation(_In_reads_(4) const float *data);


	// -----------------------------------------------
	// Service methods.
	// -----------------------------------------------

	// ------------
	// METADATA

	// Clears the queue of all transforms.
	void clear();

	// Returns the number of elements in the queue.
	unsigned int size() const;

	// Checks if the queue is currently empty.
	bool is_empty() const;


private:

	// -----------------------------------------------
	// Data members.
	// -----------------------------------------------

	// Stores position, scale, or rotation transformations.
	TransformQueue internalQueue;

	// -----------------------------------------------
	// Helper methods.
	// -----------------------------------------------

	// ------------
	// QUEUE HANDLER

	void clear(TransformQueue& queue);
	unsigned int size(const TransformQueue& queue) const;
	bool is_empty(const TransformQueue& queue) const;

	// PEEK
	const TransformPair peek(const TransformQueue& queue) const;
	void peek(TransformPair& target, const TransformQueue& queue) const;

	// POP
	void pop(TransformQueue& queue);

	// PUSH
	void push(TransformQueue& queue, TransformPair& pair);
	void push(TransformQueue& queue, TRANSFORM_TYPE type, const DirectX::XMFLOAT4& data);
	void push(TransformQueue& queue, TRANSFORM_TYPE type, const DirectX::XMFLOAT3& data);
	void push(TRANSFORM_TYPE type, const DirectX::XMFLOAT4& data);
	void push(TRANSFORM_TYPE type, const DirectX::XMFLOAT3& data);
	void push(TRANSFORM_TYPE type, float x, float y, float z);
	void push(TRANSFORM_TYPE type, float x, float y, float z, float w);
	void push(TRANSFORM_TYPE type, _In_reads_(4) const float *data);

	// ------------
	// PAIR BUILDERS
	void pair_transformation(TransformPair& target, TRANSFORM_TYPE type, const DirectX::XMFLOAT4& transformation);
	void pair_transformation(TransformPair& target, TRANSFORM_TYPE type, const DirectX::XMFLOAT3& transformation);
	void pair_position(TransformPair& target, const DirectX::XMFLOAT4 transformation);
	void pair_position(TransformPair& target, const DirectX::XMFLOAT3 transformation);
	void pair_scale(TransformPair& target, const DirectX::XMFLOAT4 transformation);
	void pair_scale(TransformPair& target, const DirectX::XMFLOAT3 transformation);
	void pair_rotation(TransformPair& target, const DirectX::XMFLOAT4 transformation);

	// ------------
	// TRANSFORM TYPE COMPARISONS
	bool is_type(TRANSFORM_TYPE a, TRANSFORM_TYPE b) const;
	bool is_type(TRANSFORM_TYPE a, TransformPair& pair) const;
};

