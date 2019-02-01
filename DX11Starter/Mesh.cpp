#include "Mesh.h"

// Constructor.

/// <summary>
/// Constructor for Mesh objects. Mesh constructor contains Vertex
/// array and Index array for the appropriate DirectX 11 data buffer.
/// </summary>
/// <param name="vertices">Vertex array.</param>
/// <param name="vertexCount">Number of vertices in the array.</param>
/// <param name="indices">Index array.</param>
/// <param name="indexCount">Number of indices in the array.</param>
/// <param name="device">Reference to the Direct3D Device.</param>
Mesh::Mesh(
	Vertex* vertices, // Vertex data.
	unsigned int vertexCount, // Number of vertices.
	unsigned int* indices, // Indices for Index Buffer.
	unsigned int indexCount, // Number of indices.
	ID3D11Device* device) // Direct3D Device pointer.
{
	// Initialize fields.
	vertexBuffer = 0;
	indexBuffer = 0;

	// Assign index count.
	this->indexCount = indexCount;

	// Assign values.
	CreateVertexBuffer(vertices, vertexCount, device);
	CreateIndexBuffer(indices, indexCount, device);
}

// Destructor.

/// <summary>
/// Destructor releases DirectX resources used by Mesh and
/// deletes outstanding objects to prevent memory leaks.
/// </summary>
Mesh::~Mesh()
{
	// Release resources.
	if (vertexBuffer) { vertexBuffer->Release(); }
	if (indexBuffer) { indexBuffer->Release(); }

	// Set to nullptr.
	vertexBuffer = nullptr;
	indexBuffer = nullptr;
}

// Accessor functions.

/// <summary>
/// Returns pointer to the vertex buffer.
/// </summary>
/// <returns>Returns Vertex Buffer pointer.</returns>
ID3D11Buffer* Mesh::GetVertexBuffer() const {
	return vertexBuffer;
}

/// <summary>
/// Returns pointer to the index buffer.
/// </summary>
/// <returns>Returns Index Buffer pointer.</returns>
ID3D11Buffer* Mesh::GetIndexBuffer() const {
	return indexBuffer;
}

/// <summary>
/// Return number of indices in index buffer.
/// </summary>
/// <returns>Return index count.</returns>
unsigned int Mesh::GetIndexCount() const {
	return indexCount;
}

// Helper functions.

/// <summary>
/// Creates the vertex buffer and assigns it to the member.
/// </summary>
/// <param name="vertices">Vertex array.</param>
/// <param name="count">Size of array.</param>
/// <param name="device">Direct3D Device pointer.</param>
void Mesh::CreateVertexBuffer(
	Vertex* vertices,
	unsigned int count,
	ID3D11Device* device) {

	// Create the buffer description.
	D3D11_BUFFER_DESC desc;
	desc.Usage = D3D11_USAGE_IMMUTABLE; // no changing after it reaches GPU.
	desc.ByteWidth = sizeof(Vertex) * count; // bytewidth = number of verts * size of single vert.
	desc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	// Create the proper struct for vertex data.
	D3D11_SUBRESOURCE_DATA initialData;
	initialData.pSysMem = vertices;

	// Create the actual buffer using the device. Pass items in by reference.
	device->CreateBuffer(&desc, &initialData, &vertexBuffer);
}

/// <summary>
/// Creates the index buffer and assigns it to the member.
/// </summary>
/// <param name="indices">Index array.</param>
/// <param name="count">Size of array.</param>
/// <param name="device">Direct3D Device pointer.</param>
void Mesh::CreateIndexBuffer(
	unsigned int* indices,
	unsigned int count,
	ID3D11Device* device) {


	// Create the buffer description.
	D3D11_BUFFER_DESC desc;
	desc.Usage = D3D11_USAGE_IMMUTABLE; // no changing after it reaches GPU.
	desc.ByteWidth = sizeof(int) * count; // bytewidth = number of indices * size of single index.
	desc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	desc.CPUAccessFlags = 0;
	desc.MiscFlags = 0;
	desc.StructureByteStride = 0;

	// Create the proper struct for index data.
	D3D11_SUBRESOURCE_DATA initialData;
	initialData.pSysMem = indices;

	// Create the actual buffer using the device. Pass items in by reference.
	device->CreateBuffer(&desc, &initialData, &indexBuffer);
}