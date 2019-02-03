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
// Check transformation data.

/// <summary>
/// Retrieve the data within a given TransformItem.
/// </summary>
/// <param name="item">Item to check.</param>
/// <returns>Returns item data.</returns>
const XMFLOAT4 TransformBuffer::get_data(const TransformItem& item)
{
	return std::get<XMFLOAT4>(item);
}

/// <summary>
/// Check if the data is zero.
/// </summary>
/// <param name="item">Item to check.</param>
/// <returns>Returns comparison result.</returns>
bool TransformBuffer::IsZeroVector(const TransformItem& item)
{
	const XMFLOAT4 data = get_data(item);
	float magnitude = (data.x + data.y + data.z + data.w);
	return (magnitude == 0.0f);
}

/// <summary>
/// Check if the data is non-zero.
/// </summary>
/// <param name="item">Item to check.</param>
/// <returns>Returns comparison result.</returns>
bool TransformBuffer::IsNonZeroVector(const TransformItem& item)
{
	return !(IsZeroVector(item));
}

// ------------
// Check if a transformation is of a particular scope.

/// <summary>
/// Retrieve the scope within a given TransformItem.
/// </summary>
/// <returns>Returns item scope.</returns>
const TransformBuffer::TransformScope TransformBuffer::get_scope(const TransformItem& item)
{
	return std::get<TransformScope>(item);
}

/// <summary>
/// Compare two transformation scopes.
/// </summary>
/// <param name="a">First scope.</param>
/// <param name="b">Second scope.</param>
/// <returns>Returns true if matched, false otherwise.</returns>
bool TransformBuffer::IsMatchingTransformScope(const TransformScope a, const TransformScope b) {
	return (a == b);
}

/// <summary>
/// Check if a transformation is of a particular scope.
/// </summary>
/// <param name="scope">Scope to check against.</param>
/// <param name="transformation">Transformation group to check.</param>
/// <returns>Returns true if the item matches the expected scope.</returns>
bool TransformBuffer::IsOfTransformScope(TransformScope scope, const TransformItem& transformation)
{
	return IsMatchingTransformScope(scope, get_scope(transformation));
}

/// <summary>
/// Check if a transformation is of a particular scope.
/// </summary>
/// <param name="transformation">Transformation item to check.</param>
/// <returns>Returns true if the item matches the expected scope.</returns>
bool TransformBuffer::IsIgnored(const TransformItem& transformation)
{
	return IsOfTransformScope(S_IGNORE, transformation);
}

/// <summary>
/// Check if a transformation is of a particular scope.
/// </summary>
/// <param name="transformation">Transformation item to check.</param>
/// <returns>Returns true if the item matches the expected scope.</returns>
bool TransformBuffer::IsAbsolute(const TransformItem& transformation)
{
	return IsOfTransformScope(S_ABSOLUTE, transformation);
}

/// <summary>
/// Check if a transformation is of a particular scope.
/// </summary>
/// <param name="transformation">Transformation item to check.</param>
/// <returns>Returns true if the item matches the expected scope.</returns>
bool TransformBuffer::IsRelative(const TransformItem& transformation)
{
	return IsOfTransformScope(S_RELATIVE, transformation);
}

// ------------
// Check if a transformation is of a particular type.

/// <summary>
/// Retrieve the type within a given TransformItem.
/// </summary>
/// <returns>Returns item type.</returns>
const TransformBuffer::TransformType TransformBuffer::get_type(const TransformItem& item)
{
	return std::get<TransformType>(item);
}

/// <summary>
/// Compare two transformation types.
/// </summary>
/// <param name="a">First type.</param>
/// <param name="b">Second type.</param>
/// <returns>Returns true if matched, false otherwise.</returns>
bool TransformBuffer::IsMatchingTransformType(const TransformType a, const TransformType b) {
	return (a == b);
}

/// <summary>
/// Check if a transformation is of a particular type.
/// </summary>
/// <param name="type">Type to check against.</param>
/// <param name="transformation">Transformation item to check.</param>
/// <returns>Returns true if the item matches the expected type.</returns>
bool TransformBuffer::IsOfTransformType(TransformType type, const TransformItem& transformation)
{
	return IsMatchingTransformType(type, get_type(transformation));
}

/// <summary>
/// Check if a transformation is of a particular type.
/// </summary>
/// <param name="transformation">Transformation item to check.</param>
/// <returns>Returns true if the item matches the expected type.</returns>
bool TransformBuffer::IsNull(const TransformItem& transformation)
{
	return IsOfTransformType(T_NONE, transformation);
}

/// <summary>
/// Check if a transformation is of a particular type.
/// </summary>
/// <param name="transformation">Transformation item to check.</param>
/// <returns>Returns true if the item matches the expected type.</returns>
bool TransformBuffer::IsPosition(const TransformItem& transformation)
{
	return IsOfTransformType(T_POSITION, transformation);
}

/// <summary>
/// Check if a transformation is of a particular type.
/// </summary>
/// <param name="transformation">Transformation item to check.</param>
/// <returns>Returns true if the item matches the expected type.</returns>
bool TransformBuffer::IsScale(const TransformItem& transformation)
{
	return IsOfTransformType(T_SCALE, transformation);
}

/// <summary>
/// Check if a transformation is of a particular type.
/// </summary>
/// <param name="transformation">Transformation item to check.</param>
/// <returns>Returns true if the item matches the expected type.</returns>
bool TransformBuffer::IsRotation(const TransformItem& transformation)
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
// TRANSFORM TYPE

/// <summary>
/// Get the type of the front element.
/// </summary>
/// <returns>Returns a Transform type.</returns>
const TransformBuffer::TransformType TransformBuffer::next_type() const
{
	return get_type(this->peek_item());
}

// ------------
// TRANSFORM SCOPE

/// <summary>
/// Get the scope of the front element.
/// </summary>
/// <returns>Returns a Transform scope.</returns>
const TransformBuffer::TransformScope TransformBuffer::next_scope() const
{
	return get_scope(this->peek_item());
}

// ------------
// TRANSFORM PAIR

/// <summary>
/// Return the top element as a raw item.
/// </summary>
/// <returns>Returns the front element.</returns>
const TransformBuffer::TransformItem& TransformBuffer::peek_item() const
{
	return this->peek(internalQueue);
}

/// <summary>
/// Return data stored in front item as a 4D vector.
/// </summary>
/// <returns>Returns data stored in the next element.</returns>
const XMFLOAT4 TransformBuffer::peek_float4() const
{
	return get_data(this->peek_item());
}

/// <summary>
/// Return data stored in front item as a 3D vector.
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
void TransformBuffer::peek(TransformItem& target) const
{
	target = this->peek_item();
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
// TRANSFORM TYPE

/// <summary>
/// Change the info for the front element.
/// </summary>
/// <param name="type">Type to be set.</param>
void TransformBuffer::change_item_type(const TransformType type)
{
	TransformItem item = this->peek_item();
	std::get<TransformType>(item) = type;
}

// ------------
// TRANSFORM SCOPE

/// <summary>
/// Change the info for the front element.
/// </summary>
/// <param name="scope">Scope to be set.</param>
void TransformBuffer::change_item_scope(const TransformScope scope)
{
	TransformItem item = this->peek_item();
	std::get<TransformScope>(item) = scope;
}

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
/// <param name="scope">Scope of transformation.</param>
void TransformBuffer::push_position(const DirectX::XMFLOAT4& source, TransformScope scope)
{
	this->push(T_POSITION, source, scope);
}

/// <summary>
/// Push transformation onto the queue.
/// </summary>
/// <param name="source">Data in transformation.</param>
/// <param name="scope">Scope of transformation.</param>
void TransformBuffer::push_position(const DirectX::XMFLOAT3& source, TransformScope scope)
{
	this->push(T_POSITION, source, scope);
}

/// <summary>
/// Push transformation onto the queue.
/// </summary>
/// <param name="x">Data in transformation.</param>
/// <param name="y">Data in transformation.</param>
/// <param name="z">Data in transformation.</param>
/// <param name="scope">Scope of transformation.</param>
void TransformBuffer::push_position(float x, float y, float z, TransformScope scope)
{
	this->push_position(XMFLOAT3(x, y, z), scope);
}

/// <summary>
/// Push transformation onto the queue.
/// </summary>
/// <param name="data">Data in transformation.</param>
/// <param name="scope">Scope of transformation.</param>
void TransformBuffer::push_position(_In_reads_(3) const float *data, TransformScope scope)
{
	this->push_position(XMFLOAT3(data), scope);
}


/// <summary>
/// Push transformation onto the queue.
/// </summary>
/// <param name="source">Data in transformation.</param>
/// <param name="scope">Scope of transformation.</param>
void TransformBuffer::push_position(const TRANSFORM& source, TransformScope scope)
{
	this->push_position(source.GetPosition(), scope);
}

// ------------
// SCALE

/// <summary>
/// Push transformation onto the queue.
/// </summary>
/// <param name="source">Data in transformation.</param>
/// <param name="scope">Scope of transformation.</param>
void TransformBuffer::push_scale(const DirectX::XMFLOAT4& source, TransformScope scope)
{
	this->push(T_SCALE, source, scope);
}

/// <summary>
/// Push transformation onto the queue.
/// </summary>
/// <param name="source">Data in transformation.</param>
/// <param name="scope">Scope of transformation.</param>
void TransformBuffer::push_scale(const DirectX::XMFLOAT3& source, TransformScope scope)
{
	this->push(T_SCALE, source, scope);
}

/// <summary>
/// Push transformation onto the queue.
/// </summary>
/// <param name="source">Data in transformation.</param>
/// <param name="scope">Scope of transformation.</param>
void TransformBuffer::push_scale(const TRANSFORM& source, TransformScope scope)
{
	this->push_scale(source.GetScale(), scope);
}

/// <summary>
/// Push transformation onto the queue.
/// </summary>
/// <param name="x">Data in transformation.</param>
/// <param name="y">Data in transformation.</param>
/// <param name="z">Data in transformation.</param>
/// <param name="scope">Scope of transformation.</param>
void TransformBuffer::push_scale(float x, float y, float z, TransformScope scope)
{
	this->push_scale(XMFLOAT3(x, y, z), scope);
}

/// <summary>
/// Push transformation onto the queue.
/// </summary>
/// <param name="data">Data in transformation.</param>
/// <param name="scope">Scope of transformation.</param>
void TransformBuffer::push_scale(_In_reads_(3) const float *data, TransformScope scope)
{
	this->push_scale(XMFLOAT3(data), scope);
}

// ------------
// ROTATION

/// <summary>
/// Push transformation onto the queue.
/// </summary>
/// <param name="source">Data in transformation.</param>
/// <param name="scope">Scope of transformation.</param>
void TransformBuffer::push_rotation(const DirectX::XMFLOAT4& source, TransformScope scope)
{
	this->push(T_ROTATION, source, scope);
}

/// <summary>
/// Push transformation onto the queue.
/// </summary>
/// <param name="source">Data in transformation.</param>
/// <param name="scope">Scope of transformation.</param>
void TransformBuffer::push_rotation(const TRANSFORM& source, TransformScope scope)
{
	this->push_rotation(source.GetRotation(), scope);
}

/// <summary>
/// Push transformation onto the queue.
/// </summary>
/// <param name="x">Data in transformation.</param>
/// <param name="y">Data in transformation.</param>
/// <param name="z">Data in transformation.</param>
/// <param name="w">Data in transformation.</param>
/// <param name="scope">Scope of transformation.</param>
void TransformBuffer::push_rotation(float x, float y, float z, float w, TransformScope scope)
{
	this->push_rotation(XMFLOAT4(x, y, z, w), scope);
}

/// <summary>
/// Push transformation onto the queue.
/// </summary>
/// <param name="data">Data in transformation.</param>
/// <param name="scope">Scope of transformation.</param>
void TransformBuffer::push_rotation(_In_reads_(4) const float *data, TransformScope scope)
{
	this->push_rotation(XMFLOAT4(data), scope);
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
/// <returns>Returns a transform item.</returns>
const TransformBuffer::TransformItem& TransformBuffer::peek(const TransformQueue& queue) const
{
	return queue.front();
}

/// <summary>
/// Wrapper for queue.front().
/// </summary>
/// <param name="target">Location to store the next element.</param>
/// <param name="queue">Queue to peek next element from.</param>
void TransformBuffer::peek(TransformItem& target, const TransformQueue& queue) const
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
/// Add item to end of queue.
/// </summary>
/// <param name="queue">Queue to modify.</param>
/// <param name="item">Pair to add.</param>
void TransformBuffer::push(TransformQueue& queue, TransformItem& item)
{
	return queue.push(item);
}

/// <summary>
/// Add item to end of queue.
/// </summary>
/// <param name="queue">Queue to modify.</param>
/// <param name="type">Pair type.</param>
/// <param name="data">Pair data.</param>
void TransformBuffer::push(TransformQueue& queue, TransformType type, const DirectX::XMFLOAT4& data, TransformScope scope)
{
	TransformItem item;
	this->item_transformation(item, type, data, scope);
	this->push(queue, item);
}

/// <summary>
/// Add item to end of queue.
/// </summary>
/// <param name="queue">Queue to modify.</param>
/// <param name="type">Pair type.</param>
/// <param name="data">Pair data.</param>
void TransformBuffer::push(TransformQueue& queue, TransformType type, const DirectX::XMFLOAT3& data, TransformScope scope)
{
	XMFLOAT4 conversion;
	TransformBuffer::ConvertTransformation(conversion, data);
	this->push(queue, type, conversion, scope);
}

/// <summary>
/// Push item of TransformType type to queue.
/// </summary>
/// <param name="type">Type to push.</param>
/// <param name="data">Data to store.</param>
void  TransformBuffer::push(TransformType type, const DirectX::XMFLOAT4& data, TransformScope scope)
{
	this->push(internalQueue, type, data, scope);
}

/// <summary>
/// Push item of TransformType type to queue.
/// </summary>
/// <param name="type">Type to push.</param>
/// <param name="data">Data to store.</param>
void  TransformBuffer::push(TransformType type, const DirectX::XMFLOAT3& data, TransformScope scope)
{
	this->push(internalQueue, type, data, scope);
}

/// <summary>
/// Push item of TransformBuffer::TransformType type to queue.
/// </summary>
/// <param name="type">Type to push.</param>
/// <param name="x">Data to store.</param>
/// <param name="y">Data to store.</param>
/// <param name="z">Data to store.</param>
void  TransformBuffer::push(TransformType type, float x, float y, float z, TransformScope scope)
{
	this->push(type, XMFLOAT3(x, y, z), scope);
}

/// <summary>
/// Push item of TransformType type to queue.
/// </summary>
/// <param name="type">Type to push.</param>
/// <param name="x">Data to store.</param>
/// <param name="y">Data to store.</param>
/// <param name="z">Data to store.</param>
/// <param name="w">Data to store.</param>
void  TransformBuffer::push(TransformType type, float x, float y, float z, float w, TransformScope scope)
{
	this->push(type, XMFLOAT4(x, y, z, w), scope);
}

/// <summary>
/// Push item of TransformType type to queue.
/// </summary>
/// <param name="type">Type to push.</param>
/// <param name="data">Data to store.</param>
void  TransformBuffer::push(TransformType type, _In_reads_(4) const float *data, TransformScope scope)
{
	this->push(type, XMFLOAT4(data), scope);
}

// ------------
// PAIR BUILDERS

/// <summary>
/// Create a TransformPair from input values.
/// </summary>
/// <param name="target">Where the resulting TransformPair is stored.</param>
/// <param name="type">Type of transformation.</param>
/// <param name="transformation">Transformation data.</param>
void TransformBuffer::item_transformation(TransformItem& target, TransformType type, const DirectX::XMFLOAT4& transformation, TransformScope scope)
{
	target = TransformItem(type, transformation, scope);
}

/// <summary>
/// Create a TransformPair from input values.
/// </summary>
/// <param name="target">Where the resulting TransformPair is stored.</param>
/// <param name="type">Type of transformation.</param>
/// <param name="transformation">Transformation data.</param>
void TransformBuffer::item_transformation(TransformItem& target, TransformType type, const DirectX::XMFLOAT3& transformation, TransformScope scope)
{
	XMFLOAT4 conversion;
	TransformBuffer::ConvertTransformation(conversion, transformation);
	this->item_transformation(target, type, conversion, scope);
}

/// <summary>
/// Create a TransformPair from input values.
/// </summary>
/// <param name="target">Where the resulting TransformPair is stored.</param>
/// <param name="transformation">Transformation data.</param>
void TransformBuffer::item_position(TransformItem& target, const DirectX::XMFLOAT4 transformation, TransformScope scope)
{
	this->item_transformation(target, T_POSITION, transformation, scope);
}

/// <summary>
/// Create a TransformPair from input values.
/// </summary>
/// <param name="target">Where the resulting TransformPair is stored.</param>
/// <param name="transformation">Transformation data.</param>
void TransformBuffer::item_position(TransformItem& target, const DirectX::XMFLOAT3 transformation, TransformScope scope)
{
	this->item_transformation(target, T_POSITION, transformation, scope);
}

/// <summary>
/// Create a TransformPair from input values.
/// </summary>
/// <param name="target">Where the resulting TransformPair is stored.</param>
/// <param name="transformation">Transformation data.</param>
void TransformBuffer::item_scale(TransformItem& target, const DirectX::XMFLOAT4 transformation, TransformScope scope)
{
	this->item_transformation(target, T_SCALE, transformation, scope);
}

/// <summary>
/// Create a TransformPair from input values.
/// </summary>
/// <param name="target">Where the resulting TransformPair is stored.</param>
/// <param name="transformation">Transformation data.</param>
void TransformBuffer::item_scale(TransformItem& target, const DirectX::XMFLOAT3 transformation, TransformScope scope)
{
	this->item_transformation(target, T_SCALE, transformation, scope);
}

/// <summary>
/// Create a TransformPair from input values.
/// </summary>
/// <param name="target">Where the resulting TransformPair is stored.</param>
/// <param name="transformation">Transformation data.</param>
void TransformBuffer::item_rotation(TransformItem& target, const DirectX::XMFLOAT4 transformation, TransformScope scope)
{
	this->item_transformation(target, T_ROTATION, transformation, scope);
}

// ------------
// TRANSFORM TYPE COMPARISONS

/// <summary>
/// Compare two related objects for transform type equality.
/// </summary>
/// <param name="a">First type.</param>
/// <param name="b">Second type.</param>
/// <returns>Returns true if types match, false otherwise.</returns>
bool TransformBuffer::is_type(TransformType a, TransformType b) const
{
	return IsMatchingTransformType(a, b);
}

/// <summary>
/// Compare two related objects for transform type equality.
/// </summary>
/// <param name="a">First type.</param>
/// <param name="b">Second type.</param>
/// <returns>Returns true if types match, false otherwise.</returns>
bool TransformBuffer::is_type(TransformType a, TransformItem& b) const
{
	return IsOfTransformType(a, b);
}

// ------------
// TRANSFORM SCOPE COMPARISONS

/// <summary>
/// Compare two related objects for transform scope equality.
/// </summary>
/// <param name="a">First scope.</param>
/// <param name="b">Second scope.</param>
/// <returns>Returns true if scopes match, false otherwise.</returns>
bool TransformBuffer::is_scope(TransformScope a, TransformScope b) const
{
	return IsMatchingTransformScope(a, b);
}

/// <summary>
/// Compare two related objects for transform scope equality.
/// </summary>
/// <param name="a">First scope.</param>
/// <param name="b">Second scope.</param>
/// <returns>Returns true if scopes match, false otherwise.</returns>
bool TransformBuffer::is_scope(TransformScope a, TransformItem& b) const
{
	return IsOfTransformScope(a, b);
}