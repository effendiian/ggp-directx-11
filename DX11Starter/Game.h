#pragma once

#include "DXCore.h"
#include "Mesh.h"
#include "GameEntity.h"
#include "SimpleShader.h"
#include "Vertex.h"
#include "Camera.h"
#include "Lights.h"
#include <DirectXMath.h>
#include <vector>
#include <map>
#include <string.h>

class Game
	: public DXCore
{

	// Key Input enum.
	typedef enum _ACTION {
		// Camera Movement.
		CAMERA_MOVE_UP,
		CAMERA_MOVE_DOWN,
		CAMERA_MOVE_FORWARD,
		CAMERA_MOVE_BACKWARD,
		CAMERA_MOVE_LEFT,
		CAMERA_MOVE_RIGHT,

		// Camera Rotation.
		CAMERA_PITCH_UP,
		CAMERA_PITCH_DOWN,
		CAMERA_TURN_LEFT, // Yaw
		CAMERA_TURN_RIGHT, // Yaw
		CAMERA_ROLL_LEFT,
		CAMERA_ROLL_RIGHT,

		// Key determines if rotation modifier has been selected.
		MODIFIER_RESET,
		MODIFIER_ROTATE
	} ACTION;

	// data.
	typedef GameEntity::MeshReference pSharedMesh;
	typedef GameEntity::GameEntityReference pUniqueGameEntity;

	// collections
	typedef std::vector<unsigned int> IndexCollection;
	typedef std::vector<Vertex> VertexCollection;
	typedef std::vector<GameEntity::MeshReference> MeshCollection;
	typedef GameEntity::GameEntityCollection GameEntityCollection;
	
	typedef std::map<ACTION, std::string> KeyMappings;
	typedef std::map<std::string, bool> KeyCodes;

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
	void CreateInput();
	void CreateMatrices();
	void CreateBasicGeometry();
	void CreateEntities();

	// Light.
	DirectionalLight directionalLight1;
	DirectionalLight directionalLight2;

	// Buffers to hold actual geometry data
	int meshCount;
	MeshCollection meshObjects; // Alias to std::vector<std::shared_ptr<Mesh>>.

	// Entities.
	int gameEntityCount;
	GameEntityCollection gameEntities;  // Alias to std::vector<std::unique_ptr<GameEntity>>.

	// Wrappers for DirectX shaders to provide simplified functionality
	SimpleVertexShader* vertexShader;
	SimplePixelShader* pixelShader;
	Material* sharedMaterial;

	// The camera.
	Camera camera;
	
	// Key mappings.
	KeyMappings keyMap;
	KeyCodes keyCodes;

	// The matrices to go from model space to screen space
	// DirectX::XMFLOAT4X4 worldMatrix;
	// DirectX::XMFLOAT4X4 viewMatrix;
	// DirectX::XMFLOAT4X4 projectionMatrix;

	// Keeps track of the old mouse position.  Useful for
	// determining how far the mouse moved in a single frame.
	POINT prevMousePos;
};

