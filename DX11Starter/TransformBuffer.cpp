// -----------------------------------------------
// Include statements.
// -----------------------------------------------
#include "TransformBuffer.h"

// -----------------------------------------------
// Namespace for math library.
// -----------------------------------------------
using namespace DirectX;

// -----------------------------------------------
// -----------------------------------------------
// Public methods.
// -----------------------------------------------
// -----------------------------------------------

// -----------------------------------------------
// Constructors.
// -----------------------------------------------

/// <summary>
/// Initialize an empty queue.
/// </summary>
TransformBuffer::TransformBuffer()
{
	// Initialize data members.
	this->internalQueue = TransformQueue();
}

// -----------------------------------------------
// Static methods.
// -----------------------------------------------

// ------------
// Check if a transformation is of a particular type.

/// <summary>
/// Compare two transformation types.
/// </summary>
/// <param name="a">First type.</param>
/// <param name="b">Second type.</param>
/// <returns>Returns true if matched, false otherwise.</returns>
bool TransformBuffer::IsMatchingTransformType(const TRANSFORM_TYPE a, const TRANSFORM_TYPE b) {
	return (a == b);
}

/// <summary>
/// Check if a transformation is of a particular type.
/// </summary>
/// <param name="type">Type to check against.</param>
/// <param name="transformation">Transformation pairing to check.</param>
/// <returns>Returns true if the pair matches the expected type.</returns>
bool TransformBuffer::IsOfTransformType(TRANSFORM_TYPE type, const TransformPair& transformation)
{
	return IsMatchingTransformType(type, transformation.first);
}

/// <summary>
/// Check if a transformation is of a particular type.
/// </summary>
/// <param name="transformation">Transformation pairing to check.</param>
/// <returns>Returns true if the pair matches the expected type.</returns>
bool TransformBuffer::IsNull(const TransformPair& transformation)
{
	return IsOfTransformType(T_NONE, transformation);
}

/// <summary>
/// Check if a transformation is of a particular type.
/// </summary>
/// <param name="transformation">Transformation pairing to check.</param>
/// <returns>Returns true if the pair matches the expected type.</returns>
bool TransformBuffer::IsPosition(const TransformPair& transformation)
{
	return IsOfTransformType(T_POSITION, transformation);
}

/// <summary>
/// Check if a transformation is of a particular type.
/// </summary>
/// <param name="transformation">Transformation pairing to check.</param>
/// <returns>Returns true if the pair matches the expected type.</returns>
bool TransformBuffer::IsScale(const TransformPair& transformation)
{
	return IsOfTransformType(T_SCALE, transformation);
}

/// <summary>
/// Check if a transformation is of a particular type.
/// </summary>
/// <param name="transformation">Transformation pairing to check.</param>
/// <returns>Returns true if the pair matches the expected type.</returns>
bool TransformBuffer::IsRotation(const TransformPair& transformation)
{
	return IsOfTransformType(T_ROTATION, transformation);
}

// ------------
// Convert float vector values.

/// <summary>
/// Convert a 4D vector into a 3D, dropping the 4th dimension.
/// </summary>
/// <param name="target">Target vector.</param>
/// <param name="source">Source vector.</param>
void TransformBuffer::ConvertTransformation(DirectX::XMFLOAT3& target, const DirectX::XMFLOAT4& source)
{
	target = XMFLOAT3(source.x, source.y, source.z);
}

/// <summary>
/// Convert a 3D vector into a 4D, adding the 4th dimension.
/// </summary>
/// <param name="target">Target vector.</param>
/// <param name="source">Source vector.</param>
/// <param name="lastValue">Value to fill fourth dimension with. 0.0f by default.</param>
void TransformBuffer::ConvertTransformation(DirectX::XMFLOAT4& target, const DirectX::XMFLOAT3& source, float lastValue)
{
	target = XMFLOAT4(source.x, source.y, source.z, lastValue);
}

// -----------------------------------------------
// Accessors.
// -----------------------------------------------

// ------------
// TRANSFORM PAIR

/// <summary>
/// Return the top element as a raw pair.
/// </summary>
/// <returns>Returns the front element.</returns>
const TransformBuffer::TransformPair TransformBuffer::peek_pair() const
{
	return this->peek(internalQueue);
}

/// <summary>
/// Return data stored in front pair as a 4D vector.
/// </summary>
/// <returns>Returns data stored in the next element.</returns>
const XMFLOAT4 TransformBuffer::peek_float4() const
{
	return this->peek_pair().second;
}

/// <summary>
/// Return data stored in front pair as a 3D vector.
/// </summary>
/// <returns>Returns data stored in the next element.</returns>
const XMFLOAT3 TransformBuffer::peek_float3() const
{
	XMFLOAT3 conversion;
	TransformBuffer::ConvertTransformation(conversion, this->peek_float4());
	return conversion;
}

/// <summary>
/// Store front of queue in target.
/// </summary>
/// <param name="target">Location to store data.</param>
void TransformBuffer::peek(TransformPair& target) const
{
	target = this->peek_pair();
}

/// <summary>
/// Store front of queue in target.
/// </summary>
/// <param name="target">Location to store data.</param>
void TransformBuffer::peek(DirectX::XMFLOAT4& target) const
{
	target = this->peek_float4();
}

/// <summary>
/// Store front of queue in target.
/// </summary>
/// <param name="target">Location to store data.</param>
void TransformBuffer::peek(DirectX::XMFLOAT3& target) const
{
	target = this->peek_float3();
}

// -----------------------------------------------
// Mutators.
// -----------------------------------------------


// ------------
// TRANSFORMPAIR

/// <summary>
/// Pop the front of the queue.
/// </summary>
void TransformBuffer::pop()
{
	this->pop(internalQueue);
}

// ------------
// POSITION

/// <summary>
/// Push transformation onto the queue.
/// </summary>
/// <param name="source">Data in transformation.</param>
void TransformBuffer::push_position(const DirectX::XMFLOAT4& source)
{
	this->push(T_POSITION, source);
}

/// <summary>
/// Push transformation onto the queue.
/// </summary>
/// <param name="source">Data in transformation.</param>
void TransformBuffer::push_position(const DirectX::XMFLOAT3& source)
{
	this->push(T_POSITION, source);
}

/// <summary>
/// Push transformation onto the queue.
/// </summary>
/// <param name="x">Data in transformation.</param>
/// <param name="y">Data in transformation.</param>
/// <param name="z">Data in transformation.</param>
void TransformBuffer::push_position(float x, float y, float z)
{
	this->push_position(XMFLOAT3(x, y, z));
}

/// <summary>
/// Push transformation onto the queue.
/// </summary>
/// <param name="data">Data in transformation.</param>
void TransformBuffer::push_position(_In_reads_(3) const float *data)
{
	this->push_position(XMFLOAT3(data));
}


/// <summary>
/// Push transformation onto the queue.
/// </summary>
/// <param name="source">Data in transformation.</param>
void TransformBuffer::push_position(const TRANSFORM& source)
{
	this->push_position(source.GetPosition());
}

// ------------
// SCALE

/// <summary>
/// Push transformation onto the queue.
/// </summary>
/// <param name="source">Data in transformation.</param>
void TransformBuffer::push_scale(const DirectX::XMFLOAT4& source)
{
	this->push(T_SCALE, source);
}

/// <summary>
/// Push transformation onto the queue.
/// </summary>
/// <param name="source">Data in transformation.</param>
void TransformBuffer::push_scale(const DirectX::XMFLOAT3& source)
{
	this->push(T_SCALE, source);
}

/// <summary>
/// Push transformation onto the queue.
/// </summary>
/// <param name="source">Data in transformation.</param>
void TransformBuffer::push_scale(const TRANSFORM& source)
{
	this->push_scale(source.GetScale());
}

/// <summary>
/// Push transformation onto the queue.
/// </summary>
/// <param name="x">Data in transformation.</param>
/// <param name="y">Data in transformation.</param>
/// <param name="z">Data in transformation.</param>
void TransformBuffer::push_scale(float x, float y, float z)
{
	this->push_scale(XMFLOAT3(x, y, z));
}

/// <summary>
/// Push transformation onto the queue.
/// </summary>
/// <param name="data">Data in transformation.</param>
void TransformBuffer::push_scale(_In_reads_(3) const float *data)
{
	this->push_scale(XMFLOAT3(data));
}

// ------------
// ROTATION

/// <summary>
/// Push transformation onto the queue.
/// </summary>
/// <param name="source">Data in transformation.</param>
void TransformBuffer::push_rotation(const DirectX::XMFLOAT4& source)
{
	this->push(T_ROTATION, source);
}

/// <summary>
/// Push transformation onto the queue.
/// </summary>
/// <param name="source">Data in transformation.</param>
void TransformBuffer::push_rotation(const TRANSFORM& source)
{
	this->push_rotation(source.GetRotation());
}

/// <summary>
/// Push transformation onto the queue.
/// </summary>
/// <param name="x">Data in transformation.</param>
/// <param name="y">Data in transformation.</param>
/// <param name="z">Data in transformation.</param>
/// <param name="w">Data in transformation.</param>
void TransformBuffer::push_rotation(float x, float y, float z, float w)
{
	this->push_rotation(XMFLOAT4(x, y, z, w));
}

/// <summary>
/// Push transformation onto the queue.
/// </summary>
/// <param name="data">Data in transformation.</param>
void TransformBuffer::push_rotation(_In_reads_(4) const float *data)
{
	this->push_rotation(XMFLOAT4(data));
}

// -----------------------------------------------
// Service methods.
// -----------------------------------------------

// ------------
// METADATA

// Clears the queue of all transforms.
void TransformBuffer::clear()
{
	clear(internalQueue);
}

/// <summary>
/// Returns the number of elements in the queue.
/// </summary>
/// <returns>Returns the number of elements in the queue.</returns>
unsigned int TransformBuffer::size() const
{
	return size(internalQueue);
}

// Checks if the queue is currently empty.
bool TransformBuffer::is_empty() const
{
	return is_empty(internalQueue);
}

// -----------------------------------------------
// -----------------------------------------------
// Private methods.
// -----------------------------------------------
// -----------------------------------------------

// -----------------------------------------------
// Helper methods.
// -----------------------------------------------

// ------------
// QUEUE HANDLER

/// <summary>
/// Clear the input queue.
/// </summary>
/// <param name="queue">Queue to clear.</param>
void TransformBuffer::clear(TransformQueue& queue)
{
	TransformQueue empty;
	std::swap(queue, empty);
}

/// <summary>
/// Wrapper for internal queue's size member.
/// </summary>
/// <param name="queue">Queue containing internal data.</param>
/// <returns>Returns number of elements in the underlying container object.</returns>
unsigned int TransformBuffer::size(const TransformQueue& queue) const
{
	return static_cast<unsigned int>(queue.size());
}

/// <summary>
/// Wrapper for internal queue's empty member.
/// </summary>
/// <param name="queue">Queue containing internal data.</param>
/// <returns>Returns true if queue size is 0, false otherwise.</returns>
bool TransformBuffer::is_empty(const TransformQueue& queue) const
{
	return queue.empty();
}

// PEEK

/// <summary>
/// Wrapper for queue.front().
/// </summary>
/// <param name="queue">Queue to peek next element from.</param>
/// <returns>Returns a transform pair.</returns>
const TransformBuffer::TransformPair TransformBuffer::peek(const TransformQueue& queue) const
{
	return queue.front();
}

/// <summary>
/// Wrapper for queue.front().
/// </summary>
/// <param name="target">Location to store the next element.</param>
/// <param name="queue">Queue to peek next element from.</param>
void TransformBuffer::peek(TransformPair& target, const TransformQueue& queue) const
{
	target = peek(queue);
}

// POP

/// <summary>
/// Pop the top most transform from the queue.
/// </summary>
/// <param name="queue">Queue to modify.</param>
void TransformBuffer::pop(TransformQueue& queue)
{
	if (!is_empty(queue)) {
		queue.pop();
	}
}

// PUSH

/// <summary>
/// Add pair to end of queue.
/// </summary>
/// <param name="queue">Queue to modify.</param>
/// <param name="pair">Pair to add.</param>
void TransformBuffer::push(TransformQueue& queue, TransformPair& pair)
{
	return queue.push(pair);
}

/// <summary>
/// Add pair to end of queue.
/// </summary>
/// <param name="queue">Queue to modify.</param>
/// <param name="type">Pair type.</param>
/// <param name="data">Pair data.</param>
void TransformBuffer::push(TransformQueue& queue, TRANSFORM_TYPE type, const DirectX::XMFLOAT4& data)
{
	TransformPair pair;
	this->pair_transformation(pair, type, data);
	this->push(queue, pair);
}

/// <summary>
/// Add pair to end of queue.
/// </summary>
/// <param name="queue">Queue to modify.</param>
/// <param name="type">Pair type.</param>
/// <param name="data">Pair data.</param>
void TransformBuffer::push(TransformQueue& queue, TRANSFORM_TYPE type, const DirectX::XMFLOAT3& data)
{
	XMFLOAT4 conversion;
	TransformBuffer::ConvertTransformation(conversion, data);
	this->push(queue, type, conversion);
}

/// <summary>
/// Push pair of TRANSFORM_TYPE type to queue.
/// </summary>
/// <param name="type">Type to push.</param>
/// <param name="data">Data to store.</param>
void  TransformBuffer::push(TRANSFORM_TYPE type, const DirectX::XMFLOAT4& data)
{
	this->push(internalQueue, type, data);
}

/// <summary>
/// Push pair of TRANSFORM_TYPE type to queue.
/// </summary>
/// <param name="type">Type to push.</param>
/// <param name="data">Data to store.</param>
void  TransformBuffer::push(TRANSFORM_TYPE type, const DirectX::XMFLOAT3& data)
{
	this->push(internalQueue, type, data);
}

/// <summary>
/// Push pair of TransformBuffer::TRANSFORM_TYPE type to queue.
/// </summary>
/// <param name="type">Type to push.</param>
/// <param name="x">Data to store.</param>
/// <param name="y">Data to store.</param>
/// <param name="z">Data to store.</param>
void  TransformBuffer::push(TRANSFORM_TYPE type, float x, float y, float z)
{
	this->push(type, XMFLOAT3(x, y, z));
}

/// <summary>
/// Push pair of TRANSFORM_TYPE type to queue.
/// </summary>
/// <param name="type">Type to push.</param>
/// <param name="x">Data to store.</param>
/// <param name="y">Data to store.</param>
/// <param name="z">Data to store.</param>
/// <param name="w">Data to store.</param>
void  TransformBuffer::push(TRANSFORM_TYPE type, float x, float y, float z, float w)
{
	this->push(type, XMFLOAT4(x, y, z, w));
}

/// <summary>
/// Push pair of TRANSFORM_TYPE type to queue.
/// </summary>
/// <param name="type">Type to push.</param>
/// <param name="data">Data to store.</param>
void  TransformBuffer::push(TRANSFORM_TYPE type, _In_reads_(4) const float *data)
{
	this->push(type, XMFLOAT4(data));
}

// ------------
// PAIR BUILDERS

/// <summary>
/// Create a TransformPair from input values.
/// </summary>
/// <param name="target">Where the resulting TransformPair is stored.</param>
/// <param name="type">Type of transformation.</param>
/// <param name="transformation">Transformation data.</param>
void TransformBuffer::pair_transformation(TransformPair& target, TRANSFORM_TYPE type, const DirectX::XMFLOAT4& transformation)
{
	target = TransformPair(type, transformation);
}

/// <summary>
/// Create a TransformPair from input values.
/// </summary>
/// <param name="target">Where the resulting TransformPair is stored.</param>
/// <param name="type">Type of transformation.</param>
/// <param name="transformation">Transformation data.</param>
void TransformBuffer::pair_transformation(TransformPair& target, TRANSFORM_TYPE type, const DirectX::XMFLOAT3& transformation)
{
	XMFLOAT4 conversion;
	TransformBuffer::ConvertTransformation(conversion, transformation);
	this->pair_transformation(target, type, conversion);
}

/// <summary>
/// Create a TransformPair from input values.
/// </summary>
/// <param name="target">Where the resulting TransformPair is stored.</param>
/// <param name="transformation">Transformation data.</param>
void TransformBuffer::pair_position(TransformPair& target, const DirectX::XMFLOAT4 transformation)
{
	this->pair_transformation(target, T_POSITION, transformation);
}

/// <summary>
/// Create a TransformPair from input values.
/// </summary>
/// <param name="target">Where the resulting TransformPair is stored.</param>
/// <param name="transformation">Transformation data.</param>
void TransformBuffer::pair_position(TransformPair& target, const DirectX::XMFLOAT3 transformation)
{
	this->pair_transformation(target, T_POSITION, transformation);
}

/// <summary>
/// Create a TransformPair from input values.
/// </summary>
/// <param name="target">Where the resulting TransformPair is stored.</param>
/// <param name="transformation">Transformation data.</param>
void TransformBuffer::pair_scale(TransformPair& target, const DirectX::XMFLOAT4 transformation)
{
	this->pair_transformation(target, T_SCALE, transformation);
}

/// <summary>
/// Create a TransformPair from input values.
/// </summary>
/// <param name="target">Where the resulting TransformPair is stored.</param>
/// <param name="transformation">Transformation data.</param>
void TransformBuffer::pair_scale(TransformPair& target, const DirectX::XMFLOAT3 transformation)
{
	this->pair_transformation(target, T_SCALE, transformation);
}

/// <summary>
/// Create a TransformPair from input values.
/// </summary>
/// <param name="target">Where the resulting TransformPair is stored.</param>
/// <param name="transformation">Transformation data.</param>
void TransformBuffer::pair_rotation(TransformPair& target, const DirectX::XMFLOAT4 transformation)
{
	this->pair_transformation(target, T_ROTATION, transformation);
}

// ------------
// TRANSFORM TYPE COMPARISONS

/// <summary>
/// Compare two related objects for transform type equality.
/// </summary>
/// <param name="a">First type.</param>
/// <param name="b">Second type.</param>
/// <returns>Returns true if types match, false otherwise.</returns>
bool TransformBuffer::is_type(TRANSFORM_TYPE a, TRANSFORM_TYPE b) const
{
	return IsMatchingTransformType(a, b);
}

/// <summary>
/// Compare two related objects for transform type equality.
/// </summary>
/// <param name="a">First type.</param>
/// <param name="b">Second type.</param>
/// <returns>Returns true if types match, false otherwise.</returns>
bool TransformBuffer::is_type(TRANSFORM_TYPE a, TransformPair& b) const
{
	return IsOfTransformType(a, b);
}