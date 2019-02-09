#pragma once

#include "DXCore.h"
#include "Mesh.h"
#include "GameEntity.h"
#include "SimpleShader.h"
#include "Vertex.h"
#include "Camera.h"
#include <DirectXMath.h>
#include <vector>

class Game
	: public DXCore
{

	// data.
	typedef DirectX::XMFLOAT4 fColor;
	typedef GameEntity::MeshReference pSharedMesh;
	typedef GameEntity::GameEntityReference pUniqueGameEntity;

	// collections
	typedef std::vector<unsigned int> IndexCollection;
	typedef std::vector<Vertex> VertexCollection;
	typedef std::vector<DirectX::XMFLOAT4> ColorCollection;
	typedef std::vector<GameEntity::MeshReference> MeshCollection;
	typedef GameEntity::GameEntityCollection GameEntityCollection;


public:
	Game(HINSTANCE hInstance);
	~Game();

	// Overridden setup and game loop methods, which
	// will be called automatically
	void Init();
	void OnResize();
	void Update(float deltaTime, float totalTime);
	void Draw(float deltaTime, float totalTime);

	// Overridden mouse input helper methods
	void OnMouseDown(WPARAM buttonState, int x, int y);
	void OnMouseUp(WPARAM buttonState, int x, int y);
	void OnMouseMove(WPARAM buttonState, int x, int y);
	void OnMouseWheel(float wheelDelta, int x, int y);
private:

	// Initialization helper methods - feel free to customize, combine, etc.
	void LoadShaders();
	void CreateMatrices();
	void CreateBasicGeometry();
	void CreateEntities();

	// Buffers to hold actual geometry data
	int meshCount;
	MeshCollection meshObjects; // Alias to std::vector<std::shared_ptr<Mesh>>.

	// Entities.
	int gameEntityCount;
	GameEntityCollection gameEntities;  // Alias to std::vector<std::unique_ptr<GameEntity>>.

	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;

	// The camera.
	Camera camera;

	// The matrices to go from model space to screen space
	// DirectX::XMFLOAT4X4 worldMatrix;
	DirectX::XMFLOAT4X4 viewMatrix;
	DirectX::XMFLOAT4X4 projectionMatrix;

	// Keeps track of the old mouse position.  Useful for
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;
};

