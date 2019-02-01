// -----------------------------------------------
// Include statements.
// -----------------------------------------------
#include "TransformQueue.h"

// -----------------------------------------------
// Namespace for math library.
// -----------------------------------------------
using namespace DirectX;

// -----------------------------------------------
// Constructors.
// -----------------------------------------------

/// <summary>
/// Initialize an empty queue.
/// </summary>
TransformQueue::TransformQueue()
{
	// Initialize data members.
	this->transformQueue = std::queue<std::pair<TRANSFORM_TYPE, XMFLOAT4>>();
}

// Copy constructor.
TransformQueue::TransformQueue(const TransformQueue& other) {

}


TransformQueue::~TransformQueue()
{
}
