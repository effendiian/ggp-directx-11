#pragma once

#include "Vertex.h"
#include <d3d11.h>

class Mesh
{
public:
	Mesh(
		Vertex* vertices,
		unsigned int vertexCount,
		unsigned int* indices,
		unsigned int indexCount,
		ID3D11Device* device);
	~Mesh();

	// Accessor methods called to return buffers.
	ID3D11Buffer* GetVertexBuffer();
	ID3D11Buffer* GetIndexBuffer();
	unsigned int GetIndexCount();

private:

	// Helper functions.
	void CreateVertexBuffer(Vertex* vertices, unsigned int count, ID3D11Device* device);
	void CreateIndexBuffer(unsigned int* indices, unsigned int count, ID3D11Device* device);

	// Buffer pointers to hold geometry data.
	ID3D11Buffer* vertexBuffer; // Stores vertices.
	ID3D11Buffer* indexBuffer; // Stores winding order.
	unsigned int indexCount; // Specifies amount of indices in the mesh's index buffer.

};

