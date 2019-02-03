#pragma once

// -----------------------------------------------
// Include statements.
// -----------------------------------------------
#include <memory>
#include <queue>
#include <tuple>
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
	/// TRANSFORM_SCOPE determines if the transformation type
	/// should be applied absolutely or relative to the
	/// previous transformations.
	/// </summary>
	typedef enum _TRANFORM_SCOPE
	{
		S_IGNORE = 0,
		S_ABSOLUTE = 1, // Command will overwrite the last transformation.
		S_RELATIVE = 2  // Command will apply to existing transformation.
	} TRANSFORM_SCOPE;

	/// <summary>
	/// Wrapper for TRANSFORM_TYPE enum.
	/// </summary>
	typedef TRANSFORM_TYPE TransformType;

	/// <summary>
	/// Wrapper for TRANSFORM_SCOPE enum.
	/// </summary>
	typedef TRANSFORM_SCOPE TransformScope;

	/// <summary>
	/// Wrapper for TransformType, TransformScope, and an XMFLOAT4 type.
	/// </summary>
	typedef std::tuple<TransformType, DirectX::XMFLOAT4, TransformScope> TransformItem;

	/// <summary>
	/// Wrapper for a queue of transformations.
	/// </summary>
	typedef std::queue<TransformItem> TransformQueue;

	// -----------------------------------------------
	// Constructors.
	// -----------------------------------------------

	// Creates an empty queue.
	TransformBuffer();

	// -----------------------------------------------
	// Static methods.
	// -----------------------------------------------

	// Check transformation data.
	static const DirectX::XMFLOAT4 get_data(const TransformItem& item);
	static bool IsZeroVector(const TransformItem& item);
	static bool IsNonZeroVector(const TransformItem& item);

	// Check if a transformation is of a particular scope.
	static const TransformScope get_scope(const TransformItem& item);
	static bool IsMatchingTransformScope(const TransformScope a, const TransformScope b);
	static bool IsOfTransformScope(TransformScope a, const TransformItem& transformation);
	static bool IsIgnored(const TransformItem& transformation);
	static bool IsAbsolute(const TransformItem& transformation);
	static bool IsRelative(const TransformItem& transformation);

	// Check if a transformation is of a particular type.
	static const TransformType get_type(const TransformItem& item);
	static bool IsMatchingTransformType(const TransformType a, const TransformType b);
	static bool IsOfTransformType(TransformType type, const TransformItem& transformation);
	static bool IsNull(const TransformItem& transformation);
	static bool IsPosition(const TransformItem& transformation);
	static bool IsScale(const TransformItem& transformation);
	static bool IsRotation(const TransformItem& transformation);

	// Convert float vector values.
	static void ConvertTransformation(DirectX::XMFLOAT3& target, const DirectX::XMFLOAT4& source);
	static void ConvertTransformation(DirectX::XMFLOAT4& target, const DirectX::XMFLOAT3& source, float lastValue = 0.0f);

	// -----------------------------------------------
	// Accessors.
	// -----------------------------------------------

	// ------------
	// TRANSFORM TYPE
	const TransformType next_type() const;

	// ------------
	// TRANSFORM SCOPE
	const TransformScope next_scope() const;

	// ------------
	// TRANSFORMPAIR
	const TransformItem& peek_item() const;
	const DirectX::XMFLOAT4 peek_float4() const;
	const DirectX::XMFLOAT3 peek_float3() const;
	void peek(TransformItem& target) const;
	void peek(DirectX::XMFLOAT4& target) const;
	void peek(DirectX::XMFLOAT3& target) const;

	// -----------------------------------------------
	// Mutators.
	// -----------------------------------------------

	// ------------
	// TRANSFORM TYPE
	void change_item_type(const TransformType type);

	// ------------
	// TRANSFORM SCOPE
	void change_item_scope(const TransformScope scope);

	// ------------
	// TRANSFORMPAIR
	void pop();

	// ------------
	// POSITION
	void push_position(const DirectX::XMFLOAT4& source, TransformScope scope = S_ABSOLUTE);
	void push_position(const DirectX::XMFLOAT3& source, TransformScope scope = S_ABSOLUTE);
	void push_position(const TRANSFORM& source, TransformScope scope = S_ABSOLUTE);
	void push_position(float x, float y, float z, TransformScope scope = S_ABSOLUTE);
	void push_position(_In_reads_(3) const float *data, TransformScope scope = S_ABSOLUTE);

	// ------------
	// SCALE
	void push_scale(const DirectX::XMFLOAT4& source, TransformScope scope = S_ABSOLUTE);
	void push_scale(const DirectX::XMFLOAT3& source, TransformScope scope = S_ABSOLUTE);
	void push_scale(const TRANSFORM& source, TransformScope scope = S_ABSOLUTE);
	void push_scale(float x, float y, float z, TransformScope scope = S_ABSOLUTE);
	void push_scale(_In_reads_(3) const float *data, TransformScope scope = S_ABSOLUTE);

	// ------------
	// ROTATION
	void push_rotation(const DirectX::XMFLOAT4& source, TransformScope scope = S_ABSOLUTE);
	void push_rotation(const TRANSFORM& source, TransformScope scope = S_ABSOLUTE);
	void push_rotation(float x, float y, float z, float w, TransformScope scope = S_ABSOLUTE);
	void push_rotation(_In_reads_(4) const float *data, TransformScope scope = S_ABSOLUTE);


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
	const TransformItem& peek(const TransformQueue& queue) const;
	void peek(TransformItem& target, const TransformQueue& queue) const;

	// POP
	void pop(TransformQueue& queue);

	// PUSH
	void push(TransformQueue& queue, TransformItem& item);
	void push(TransformQueue& queue, TransformType type, const DirectX::XMFLOAT4& data, TransformScope scope = S_ABSOLUTE);
	void push(TransformQueue& queue, TransformType type, const DirectX::XMFLOAT3& data, TransformScope scope = S_ABSOLUTE);
	void push(TransformType type, const DirectX::XMFLOAT4& data, TransformScope scope = S_ABSOLUTE);
	void push(TransformType type, const DirectX::XMFLOAT3& data, TransformScope scope = S_ABSOLUTE);
	void push(TransformType type, float x, float y, float z, TransformScope scope = S_ABSOLUTE);
	void push(TransformType type, float x, float y, float z, float w, TransformScope scope = S_ABSOLUTE);
	void push(TransformType type, _In_reads_(4) const float *data, TransformScope scope = S_ABSOLUTE);

	// ------------
	// ITEM BUILDERS
	void item_transformation(TransformItem& target, TransformType type, const DirectX::XMFLOAT4& transformation, TransformScope scope = S_ABSOLUTE);
	void item_transformation(TransformItem& target, TransformType type, const DirectX::XMFLOAT3& transformation, TransformScope scope = S_ABSOLUTE);
	void item_position(TransformItem& target, const DirectX::XMFLOAT4 transformation, TransformScope scope = S_ABSOLUTE);
	void item_position(TransformItem& target, const DirectX::XMFLOAT3 transformation, TransformScope scope = S_ABSOLUTE);
	void item_scale(TransformItem& target, const DirectX::XMFLOAT4 transformation, TransformScope scope = S_ABSOLUTE);
	void item_scale(TransformItem& target, const DirectX::XMFLOAT3 transformation, TransformScope scope = S_ABSOLUTE);
	void item_rotation(TransformItem& target, const DirectX::XMFLOAT4 transformation, TransformScope scope = S_ABSOLUTE);

	// ------------
	// TRANSFORM TYPE COMPARISONS
	bool is_type(TransformType a, TransformType b) const;
	bool is_type(TransformType a, TransformItem& item) const;

	// ------------
	// TRANSFORM SCOPE COMPARISONS
	bool is_scope(TransformScope a, TransformScope b) const;
	bool is_scope(TransformScope a, TransformItem& item) const;
};

