#include "Game.h"
#include "Vertex.h"
#include "Camera.h"
#include <cstdlib>
#include <map>

// For the DirectX Math library
using namespace DirectX;

// --------------------------------------------------------
// Constructor
//
// DXCore (base class) constructor will set up underlying fields.
// DirectX itself, and our window, are not ready yet!
//
// hInstance - the application's OS-level handle (unique ID)
// --------------------------------------------------------
Game::Game(HINSTANCE hInstance)
	: DXCore(
		hInstance,		// The application's handle
		"DirectX Game",	   	// Text for the window's title bar
		1280,			// Width of the window's client area
		720,			// Height of the window's client area
		true) // Show extra stats (fps) in title bar?
{
	// -----------------
	// Initialize fields

	// Initialize shaders.
	vertexShader = 0;
	pixelShader = 0;
	sharedMaterial = 0;

	directionalLight1 = DirectionalLight{
		XMFLOAT4(0.6f, 0.1f, 0.1f, 1.0f),
		XMFLOAT4(0.1f, 0.24f, 1.0f, 1.0f),
		XMFLOAT3(1.0f, 0.0f, 1.0f)
	};

	directionalLight2 = DirectionalLight{
		XMFLOAT4(0.1f, 0.1f, 0.5f, 1.0f),
		XMFLOAT4(1.0f, 0.6f, 0.5f, 1.0f),
		XMFLOAT3(-1.0f, -1.0f, 0.0f)
	};

	// Initialize
	// worldMatrix = XMFLOAT4X4();
	// viewMatrix = XMFLOAT4X4();
	// projectionMatrix = XMFLOAT4X4();
	camera = Camera(CameraOptions::GetDefaultCameraOptions(), XMFLOAT3(0.0f, 0.0f, -5.0f)); // Default camera settings.
	prevMousePos = POINT();

	// Initialize the key mapping.
	keyMap = KeyMappings();
	keyCodes = KeyCodes();

	// Initialize the meshes.
	meshCount = 3;
	gameEntityCount = 18;

	meshObjects = MeshCollection();
	gameEntities = GameEntityCollection();

#if defined(DEBUG) || defined(_DEBUG)
	// -----------------
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("| Foundations of Game Graphics Programming: ---------------------------------- |\n");
	printf("| Console window created successfully.  Feel free to printf() here.            |\n");
	printf("| ---------------------------------------------------------------------------- |\n");
	printf("| Controls: ------------------------------------------------------------------ |\n");
	printf("| ------------ MOVEMENT [ X ('A'/'D') | Y ('W'/'S') | Z ('Q'/'E') ] ---------- |\n");
	// printf("| ----------------------- SCALE 'F'-key + [ ('A'/'D') ] ---------------------- |\n");
	printf("| -- ROTATION 'R'-key + [ YAW ('A'/'D') | PITCH ('W'/'S') | ROLL ('Q'/'E') ] - |\n");
	printf("| ---------------------------------------------------------------------------- |\n");
#endif

}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{

	// -----
	// Clear the collections and swap with empty collection to de-allocate.

	// Clear up any remaining pointers.
	for (int i = 0; i < gameEntityCount; i++)
	{
		gameEntities[i].reset();
	}

	// Clean up game entities.
	gameEntities.clear();
	GameEntityCollection().swap(gameEntities);

	// Clear up any remaining pointers.
	for (int i = 0; i < meshCount; i++)
	{
		meshObjects[i].reset();
	}

	// Clean up the mesh entities.
	meshObjects.clear();
	MeshCollection().swap(meshObjects);


	// Delete our simple shader objects, which
	// will clean up their own internal DirectX stuff
	delete vertexShader;
	delete pixelShader;
	delete sharedMaterial;
}

// --------------------------------------------------------
// Called once per program, after DirectX and the window
// are initialized but before the game loop.
// --------------------------------------------------------
void Game::Init()
{
	// Helper methods for loading shaders, creating some basic
	// geometry to draw and some simple camera matrices.
	//  - You'll be expanding and/or replacing these later
	LoadShaders();
	CreateInput();
	CreateMatrices();
	CreateBasicGeometry();
	CreateEntities();

	// Tell the input assembler stage of the pipeline what kind of
	// geometric primitives (points, lines or triangles) we want to draw.
	// Essentially: "What kind of shape should the GPU draw with our data?"
	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

// --------------------------------------------------------
// Loads shaders from compiled shader object (.cso) files using
// my SimpleShader wrapper for DirectX shader manipulation.
// - SimpleShader provides helpful methods for sending
//   data to individual variables on the GPU
// --------------------------------------------------------
void Game::LoadShaders()
{
	vertexShader = new SimpleVertexShader(device, context);
	vertexShader->LoadShaderFile(L"VertexShader.cso");

	pixelShader = new SimplePixelShader(device, context);
	pixelShader->LoadShaderFile(L"PixelShader.cso");

	// Create material.
	sharedMaterial = new Material(*vertexShader, *pixelShader);
}

void Game::CreateInput() 
{
	// Assign mappings.

	// Camera movement.
	keyMap[ACTION::CAMERA_MOVE_UP] = "SPACEBAR";
	keyMap[ACTION::CAMERA_MOVE_DOWN] = 'X';
	keyMap[ACTION::CAMERA_MOVE_FORWARD] = 'W';
	keyMap[ACTION::CAMERA_MOVE_BACKWARD] = 'S';
	keyMap[ACTION::CAMERA_MOVE_LEFT] = 'A';
	keyMap[ACTION::CAMERA_MOVE_RIGHT] = 'D';

	keyMap[ACTION::CAMERA_TURN_LEFT] = 'Q';
	keyMap[ACTION::CAMERA_TURN_RIGHT] = 'E';
	keyMap[ACTION::CAMERA_PITCH_UP] = "RW";
	keyMap[ACTION::CAMERA_PITCH_DOWN] = "RS";
	keyMap[ACTION::CAMERA_ROLL_LEFT] = "RA";
	keyMap[ACTION::CAMERA_ROLL_RIGHT] = "RD";

	keyMap[ACTION::MODIFIER_ROTATE] = "R";
	keyMap[ACTION::MODIFIER_RESET] = "TAB";

	// Assign codes by looping through the mappings.
	for (auto const& mapping : keyMap)
	{
		// Get the key and the associated value.
		ACTION key = mapping.first;
		std::string input = mapping.second;

		// Assign all values to false.
		if (input.length() > 0) {
			keyCodes[input] = false;
		}
	}
}

// --------------------------------------------------------
// Initializes the matrices necessary to represent our geometry's
// transformations and our 3D camera
// --------------------------------------------------------
void Game::CreateMatrices()
{
	// Initial updates to create the matrices.
	camera.GetTransform().SetPosition(XMFLOAT3(0.0f, 0.0f, -5.0f)); // Absolutely move camera back.
	camera.UpdateViewMatrix();
	camera.UpdateProjectionMatrix();
		
	// Set up world matrix
	// - In an actual game, each object will need one of these and they should
	//    update when/if the object moves (every frame)
	// - You'll notice a "transpose" happening below, which is redundant for
	//    an identity matrix.  This is just to show that HLSL expects a different
	//    matrix (column major vs row major) than the DirectX Math library
	// XMMATRIX W = XMMatrixIdentity();
	// XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W)); // Transpose for HLSL!

	// Create the View matrix
	// - In an actual game, recreate this matrix every time the camera
	//    moves (potentially every frame)
	// - We're using the LOOK TO function, which takes the position of the
	//    camera and the direction vector along which to look (as well as "up")
	// - Another option is the LOOK AT function, to look towards a specific
	//    point in 3D space
	// XMVECTOR pos = XMVectorSet(0, 0, -5, 0);
	// XMVECTOR dir = XMVectorSet(0, 0, 1, 0);
	// XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	// XMMATRIX V = XMMatrixLookToLH(
	// 	pos,     // The position of the "camera"
	// 	dir,     // Direction the camera is looking
	// 	up);     // "Up" direction in 3D space (prevents roll)
	// XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V)); // Transpose for HLSL!

	// Create the Projection matrix
	// - This should match the window's aspect ratio, and also update anytime
	//    the window resizes (which is already happening in OnResize() below)
	// XMMATRIX P = XMMatrixPerspectiveFovLH(
	// 	0.25f * 3.1415926535f,		// Field of View Angle
	// 	(float)width / height,		// Aspect ratio
	// 	0.1f,						// Near clip plane distance
	// 	100.0f);					// Far clip plane distance
	// XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
	
}

// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{
	// Create values for normals and uvs.
	XMFLOAT3 n (0.0f, 0.0f, -1.0f);
	XMFLOAT2 uv (0.0f, 0.0f);

	// Set up the vertices of the triangle we would like to draw
	// - We're going to copy this array, exactly as it exists in memory
	//    over to a DirectX-controlled data structure (the vertex buffer)
	VertexCollection vertices[] = {

		// Triangle
		// 0: (-3, -2, 0)
		// 1: (3, -2, 0)
		// 2: (0, 3, 0)
		{
			Vertex({ XMFLOAT3(-3.0f, -2.0f, +0.0f), n, uv }),
			Vertex({ XMFLOAT3(+3.0f, -2.0f, +0.0f), n, uv }),
			Vertex({ XMFLOAT3(+0.0f, +3.0f, +0.0f), n, uv }),
		},

		// Hexagon
		// 0: (-2, 5, 0)
		// 1: (2, 5, 0)
		// 2: (6, 0, 0)
		// 3: (2, -5, 0)
		// 4: (-2, -5, 0)
		// 5: (-6, 0, 0)
		{
			Vertex({ XMFLOAT3(-2.5f, +5.0f, +0.0f), n, uv }),
			Vertex({ XMFLOAT3(+2.5f, +5.0f, +0.0f), n, uv }),
			Vertex({ XMFLOAT3(+6.0f, +0.0f, +0.0f), n, uv }),
			Vertex({ XMFLOAT3(+2.5f, -5.0f, +0.0f), n, uv }),
			Vertex({ XMFLOAT3(-2.5f, -5.0f, +0.0f), n, uv }),
			Vertex({ XMFLOAT3(-6.0f, +0.0f, +0.0f), n, uv }),
		},

		// Square
		// 0: (-2, 2, 0)
		// 1: (2, 2, 0)
		// 2: (2, -2, 0)
		// 3: (-2, -2, 0)
		{
			Vertex({ XMFLOAT3(-2.0f, +2.0f, +0.0f), n, uv }),
			Vertex({ XMFLOAT3(+2.0f, +2.0f, +0.0f), n, uv }),
			Vertex({ XMFLOAT3(+2.0f, -2.0f, +0.0f), n, uv }),
			Vertex({ XMFLOAT3(-2.0f, -2.0f, +0.0f), n, uv }),
		},

		/*

		{ Vertex({ XMFLOAT3(-2.0f, +0.5f, +0.0f), red }),
		  Vertex({ XMFLOAT3(-2.0f, +1.5f, +0.0f), blue }),
		  Vertex({ XMFLOAT3(-1.5f, +0.5f, +0.0f), green }) },

		{ Vertex({ XMFLOAT3(+0.0f, +0.0f, +0.0f), maroon }),
		  Vertex({ XMFLOAT3(+0.5f, +1.0f, +0.0f), silver }),
		  Vertex({ XMFLOAT3(+0.5f, +0.5f, +0.0f), aqua }),
		  Vertex({ XMFLOAT3(-1.0f, +0.5f, +0.0f), red })},

		{ Vertex({ XMFLOAT3(+1.0f, +1.0f, +0.0f), fuchsia }),
		  Vertex({ XMFLOAT3(+2.0f, +1.0f, +0.0f), white }),
		  Vertex({ XMFLOAT3(+2.0f, -2.0f, +0.0f), olive }),
		  Vertex({ XMFLOAT3(+1.0f, -2.0f, +0.0f), maroon })},*/
	};


	// Set up the indices, which tell us which vertices to use and in which order
	// - This is somewhat redundant for just 3 vertices (it's a simple example)
	// - Indices are technically not required if the vertices are in the buffer
	//    in the correct order and each one will be used exactly once
	// - But just to see how it's done...
	IndexCollection indices[] = {
		{ 2, 1, 0,
		  0, 1, 2 },
		{ 0, 4, 5, 0, 3, 4, 0, 1, 3, 1, 2, 3,
		  3, 2, 1, 3, 1, 0, 4, 3, 0, 5, 4, 0 },
		{ 0, 1, 3, 1, 2, 3,
		  3, 2, 1, 3, 1, 0 },
	};

	// Get the mesh count.
	meshCount = static_cast<int>(sizeof indices / sizeof indices[0]);

	// Create meshes.
	for (int i = 0; i < meshCount; i++)
	{
		// Convert vectors back into pointer array (so we don't have to rewrite the Mesh class).
		// Notes on doing this found here: https://stackoverflow.com/questions/2923272/how-to-convert-vector-to-array
		// Spec says that vectors are guaranteed to be contiguous in memory; so we can get the address to the first element.

		// Get mesh data.
		Vertex* vertexPointer = vertices[i].data();
		unsigned int vertexCount = static_cast<unsigned int>(vertices[i].size());
		unsigned int* indexPointer = indices[i].data();
		unsigned int indexCount = static_cast<unsigned int>(indices[i].size());

		// Prepare a mesh via mesh pointer.
		meshObjects.push_back(std::make_shared<Mesh>(
			vertexPointer, vertexCount,
			indexPointer, indexCount,
			device)
		);
	}

	// Add new geometry.
	std::vector<std::string> filepaths;
	filepaths.push_back("../Assets/Models/helix.obj");
	filepaths.push_back("../Assets/Models/cone.obj");
	filepaths.push_back("../Assets/Models/cube.obj");
	filepaths.push_back("../Assets/Models/cylinder.obj");
	filepaths.push_back("../Assets/Models/sphere.obj");
	filepaths.push_back("../Assets/Models/torus.obj");

	// Loop through objects.
	for (int m = 0; m < filepaths.size(); m++) {
		char* charArr = &(filepaths[m])[0];
		meshCount++;
		meshObjects.push_back(std::make_shared<Mesh>(charArr, device));
	}
}

void Game::CreateEntities() {

	// Create entity collection.
	gameEntities = GameEntityCollection();

	// Cycle through meshes, until entity count is fulfilled.
	int meshID = -1;
	for (int i = 0; i < gameEntityCount; i++)
	{
		// Check if meshID is out of bounds, and set to zero if so.
		meshID = (meshID >= meshCount || meshID < 0) ? 0 : meshID;
		pSharedMesh mesh = meshObjects[meshID]; // Select mesh.
		meshID++; // Increment mesh ID counter for next loop through.

		// Randomize starting transformation.
		// XMFLOAT3 position;
		// position.x = static_cast<float>(rand() % 5) - static_cast<float>(rand() % 5);
		// position.y = static_cast<float>(rand() % 5) - static_cast<float>(rand() % 5);
		// position.z = static_cast<float>(rand() % 5);


		// Create the transformations.
		XMFLOAT3 bounds[] = {
			XMFLOAT3(-2.5f, -2.5f, 1.0f),
			XMFLOAT3(2.5f, 2.5f, 15.0f)
		};

		XMFLOAT3 position = GameEntity::GetRandomTransform(bounds[0], bounds[1]);
		XMFLOAT3 scale = GameEntity::GetRandomTransform(0.1f, 0.3f);

		// Create an entity with the appropriate mesh.
		pUniqueGameEntity entity(new GameEntity(*sharedMaterial, mesh,
			position.x, position.y, position.z,
			scale.x, scale.y, scale.z)
		);

		float percentage = ((float)(i) / (float)(gameEntityCount)) * 0.5f;
		entity->SetColor(XMFLOAT4(percentage * 0.5f, 0.5f + percentage, percentage, 0.1f));

		// Print info about the entity.
		// printf("{| Position [%d]: < %5.3f, %5.3f, %5.3f > | ", i, position.x, position.y, position.z);
		// printf("Scale [%d]: < %5.3f, %5.3f, %5.3f > | }\n", i, scale.x, scale.y, scale.z);


		gameEntities.push_back(std::move(entity));
	}
}

// --------------------------------------------------------
// Handle resizing DirectX "stuff" to match the new window size.
// For instance, updating our projection matrix's aspect ratio.
// --------------------------------------------------------
void Game::OnResize()
{
	// Handle base-level DX resize stuff
	DXCore::OnResize();

	// Update the camera size.
	this->camera.SetDimensions((float) width, (float) height); // Updates the aspect ratio and updates the projection matrix.

	// ORIGINAL: Update our projection matrix since the window size changed
	/* XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,	// Field of View Angle
		(float)width / height,	// Aspect ratio
		0.1f,				  	// Near clip plane distance
		100.0f);			  	// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL! */
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	// Quit if the escape key is pressed
	if (GetAsyncKeyState(VK_ESCAPE)) { Quit(); }

	// No input on this frame flag.
	bool keyPressed = false;

	// Update key input states.
	for (auto const& keys : keyCodes) 
	{
		// Get the input string.
		std::string input = keys.first;

		// If input key is zero-length string, continue.
		if (input.length() == 0) { continue; }
		
		// Update states.
		// Handle special cases.
		if (input == "SPACEBAR") 
		{
			keyCodes[input] = GetAsyncKeyState(VK_SPACE);
			keyPressed = true;
		}
		else if (input == "TAB") 
		{
			keyCodes[input] = GetAsyncKeyState(VK_TAB);
			keyPressed = true;
		}
		else
		{
			// Placeholder value.
			bool check = true;

			// Each character must be valid in order for check to succeed.
			for (int i = 0; i < input.length(); i++) 
			{
				check = check && (GetAsyncKeyState(input[i]) & 0x8000);
			}

			keyCodes[input] = check;
			keyPressed = check ? true : keyPressed;
		}	
	}
	
	// Calculate the speed.
	float speed = 5.0f;
	float deltaSpeed = speed * deltaTime;

	// Update camera rotation and movement.
	float secondsPerRotation = 5.0f;
	float angularSpeedInDegrees = 360.0f / secondsPerRotation;
	float angularSpeedInRadians = angularSpeedInDegrees * (float)(PI / 180.0f);
	float deltaRadians = angularSpeedInRadians * deltaTime;

	// Prepare changing values.
	XMFLOAT3 cam_deltaPosition(0.0f, 0.0f, 0.0f); // Initialize value for camera movement.
	XMFLOAT3 cam_deltaRotation(0.0f, 0.0f, 0.0f); // Initialize value for camera rotation.

	// Set up skip if no key pressed.
	if (keyPressed)
	{
		bool rotationModifier = keyCodes[keyMap[ACTION::MODIFIER_ROTATE]];

		// Check for keyboard input.
		for (auto const& mapping : keyMap)
		{
			// Get the key and the associated value.
			ACTION key = mapping.first;
			std::string input = mapping.second;
			bool keyDown = keyCodes[input];

			// If a key has been pressed.
			if (keyDown)
			{
				if (key == ACTION::MODIFIER_RESET)
				{
					printf("Camera > Reset [%s] \n", input.c_str());
					camera.Reset();
				}
				else 
				{
					// Switch actions based on modifiers.
					if (rotationModifier) 
					{
						/* CAMERA ROTATION */
						switch (key)
						{
							// Rotation.
						case ACTION::CAMERA_TURN_RIGHT:
							printf("Camera > Turn > Right [%s] \n", input.c_str());
							cam_deltaRotation.x += deltaRadians;
							break;
						case ACTION::CAMERA_TURN_LEFT:
							printf("Camera > Turn > Left [%s] \n", input.c_str());
							cam_deltaRotation.x -= deltaRadians;
							break;
						case ACTION::CAMERA_PITCH_UP:
							printf("Camera > Pitch > Up [%s] \n", input.c_str());
							cam_deltaRotation.y -= deltaRadians;
							break;
						case ACTION::CAMERA_PITCH_DOWN:
							printf("Camera > Pitch > Down [%s] \n", input.c_str());
							cam_deltaRotation.y += deltaRadians;
							break;
						case ACTION::CAMERA_ROLL_RIGHT:
							printf("Camera > Roll > Right [%s] \n", input.c_str());
							cam_deltaRotation.z -= deltaRadians;
							break;
						case ACTION::CAMERA_ROLL_LEFT:
							printf("Camera > Roll > Left [%s] \n", input.c_str());
							cam_deltaRotation.z += deltaRadians;
							break;
						case ACTION::MODIFIER_ROTATE:
							printf("Rotation Modifier [%s] \n", input.c_str());
						}
					}
					else
					{
						/* CAMERA MOVEMENT */
						switch (key)
						{
							// Rotation.
						case ACTION::CAMERA_TURN_RIGHT:
							printf("Camera > Turn > Right [%s] \n", input.c_str());
							cam_deltaRotation.x += deltaRadians;
							break;
						case ACTION::CAMERA_TURN_LEFT:
							printf("Camera > Turn > Left [%s] \n", input.c_str());
							cam_deltaRotation.x -= deltaRadians;
							break;

							// Movement.
						case ACTION::CAMERA_MOVE_UP:
							printf("Camera > Move > Up [%s] \n", input.c_str());
							cam_deltaPosition.y += deltaSpeed; // Movement regardless of position.
							break;
						case ACTION::CAMERA_MOVE_DOWN:
							printf("Camera > Move > Down [%s] \n", input.c_str());
							cam_deltaPosition.y -= deltaSpeed; // Movement regardless of position.
							break;
						case ACTION::CAMERA_MOVE_FORWARD:
							printf("Camera > Move > Forward [%s] \n", input.c_str());

							{
								// Get the current heading.
								XMFLOAT3 forward_safe = camera.GetTransform().GetHeading();
								forward_safe.x *= deltaSpeed;
								forward_safe.y *= deltaSpeed;
								forward_safe.z *= deltaSpeed;

								// Add to change in position.
								cam_deltaPosition.x += forward_safe.x;
								cam_deltaPosition.y += forward_safe.y;
								cam_deltaPosition.z += forward_safe.z;
							}

							break;
						case ACTION::CAMERA_MOVE_BACKWARD:
							printf("Camera > Move > Backward [%s] \n", input.c_str());

							{
								// Get the current heading.
								XMFLOAT3 backward_safe = camera.GetTransform().GetHeading();
								backward_safe.x *= -deltaSpeed;
								backward_safe.y *= -deltaSpeed;
								backward_safe.z *= -deltaSpeed;

								// Add to change in position.
								cam_deltaPosition.x += backward_safe.x; // We still add, even if backward is negative, because the deltaSpeed is already negated above.
								cam_deltaPosition.y += backward_safe.y; // We still add, even if backward is negative, because the deltaSpeed is already negated above.
								cam_deltaPosition.z += backward_safe.z; // We still add, even if backward is negative, because the deltaSpeed is already negated above.
							}

							break;
						case ACTION::CAMERA_MOVE_LEFT:
							printf("Camera > Move > Left [%s] \n", input.c_str());

							{
								// Get the left vector.
								XMFLOAT3 forward_safe = camera.GetTransform().GetHeading();
								XMFLOAT3 global_up_safe = UnitVector::GetDefaultUp().Get();
						
								// Calculate the right vector.
								XMVECTOR forward = XMLoadFloat3(&forward_safe);
								XMVECTOR global_up = XMLoadFloat3(&global_up_safe);
								XMVECTOR right_vector = XMVector3Cross(forward, global_up);
						
								// Store the vector.
								XMFLOAT3 right_safe;
								XMStoreFloat3(&right_safe, right_vector);

								// Negate the vector for the left.
								XMFLOAT3 left_safe(0.0f, 0.0f, 0.0f);
								left_safe.x = (right_safe.x * deltaSpeed);
								left_safe.y = (right_safe.y * deltaSpeed);
								left_safe.z = (right_safe.z * deltaSpeed);

								// Apply transformation to delta movement.
								cam_deltaPosition.x += left_safe.x;
								cam_deltaPosition.y += left_safe.y;
								cam_deltaPosition.z += left_safe.z;
							}

							break;
						case ACTION::CAMERA_MOVE_RIGHT:
							printf("Camera > Move > Right [%s] \n", input.c_str());

							{
								// Get the right vector.
								XMFLOAT3 forward_safe = camera.GetTransform().GetHeading();
								XMFLOAT3 global_up_safe = UnitVector::GetDefaultUp().Get();

								// Calculate the right vector.
								XMVECTOR forward = XMLoadFloat3(&forward_safe);
								XMVECTOR global_up = XMLoadFloat3(&global_up_safe);
								XMVECTOR right_vector = XMVector3Cross(forward, global_up);

								// Store the vector.
								XMFLOAT3 right_safe;
								XMStoreFloat3(&right_safe, right_vector);

								// Scale the right vector.
								right_safe.x *= -deltaSpeed;
								right_safe.y *= -deltaSpeed;
								right_safe.z *= -deltaSpeed;

								// Apply transformation to delta movement.
								cam_deltaPosition.x += right_safe.x;
								cam_deltaPosition.y += right_safe.y;
								cam_deltaPosition.z += right_safe.z;
							}

							break;
						}
					}
			

					// Update the camera.
					camera.UpdatePosition(cam_deltaPosition);
					camera.UpdateRotation(cam_deltaRotation);
				}
			}

		}
	}

	// Update the entities.
	for (int i = 0; i < gameEntityCount; i++)
	{
		gameEntities[i]->Update(deltaTime, totalTime);
	}



	/*
	float scaleMagnitude = 2.0f * deltaTime;
	float magnitude = 5.0f * deltaTime;
	float x = 0.0f, y = 0.0f, z = 0.0f;
	float deltaScale = 0.0f;
	float pitchY = 0.0f, yawX = 0.0f, rollZ = 0.0f;


	if (GetAsyncKeyState('R') & 0x8000) {
		printf("R key is down.\n");

		if (GetAsyncKeyState('E') & 0x8000) {
			printf("R+E key is down.\n");
			yawX = magnitude;
		}

		if (GetAsyncKeyState('Q') & 0x8000) {
			printf("R+Q key is down.\n");
			yawX = -magnitude;
		}

		if (GetAsyncKeyState('W') & 0x8000) {
			printf("R+W key is down.\n");
			pitchY = magnitude;
		}

		if (GetAsyncKeyState('S') & 0x8000) {
			printf("R+S key is down.\n");
			pitchY = -magnitude;
		}

		if (GetAsyncKeyState('A') & 0x8000) {
			printf("R+A key is down.\n");
			rollZ = -magnitude;
		}

		if (GetAsyncKeyState('D') & 0x8000) {
			printf("R+D key is down.\n");
			rollZ = magnitude;
		}
	}
	else if (GetAsyncKeyState('F') & 0x8000) {
		printf("F key is down.\n");

		if (GetAsyncKeyState('A') & 0x8000) {
			printf("F+A key is down.\n");
			deltaScale = -scaleMagnitude;
		}

		if (GetAsyncKeyState('D') & 0x8000) {
			printf("F+D key is down.\n");
			deltaScale = scaleMagnitude;
		}
	}
	else
	{
		if (GetAsyncKeyState('E') & 0x8000) {
			printf("E key is down.\n");
			z = magnitude;
		}

		if (GetAsyncKeyState('Q') & 0x8000) {
			printf("Q key is down.\n");
			z = -magnitude;
		}

		if (GetAsyncKeyState('W') & 0x8000) {
			printf("W key is down.\n");
			y = magnitude;
		}

		if (GetAsyncKeyState('S') & 0x8000) {
			printf("S key is down.\n");
			y = -magnitude;
		}

		if (GetAsyncKeyState('A') & 0x8000) {
			printf("A key is down.\n");
			x = -magnitude;
		}

		if (GetAsyncKeyState('D') & 0x8000) {
			printf("D key is down.\n");
			x = magnitude;
		}
	}

	// Update each of the entities.
	for (int i = 0; i < gameEntityCount; i++)
	{
		// gameEntities[i]->Move(x, y, z);
		// gameEntities[i]->Scale(deltaScale, deltaScale, deltaScale);
		// gameEntities[i]->Rotate(pitchY, yawX, rollZ);
		// gameEntities[i]->Update(deltaTime, totalTime);
		// printf("Entity[%d] Position: < %4.2f, %4.2f > \n", i, gameEntities[i]->GetPosition().x, gameEntities[i]->GetPosition().y);
	}
	// printf("\n");

	// Update camera rotation and movement.
	// float secondsPerCycle = 10;
	// float degree = 360.0f / secondsPerCycle;
	// float angle = degree * (float)(PI / 180.0f);

	// camera.UpdatePosition(deltaTime, deltaScale, XMFLOAT3(0.0f, 0.0f, -5.0f));
	// camera.UpdateRotation(deltaTime, deltaScale, XMFLOAT3(0.0f, 0.0f, 0.0f)); 
	*/

}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// ----------
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	// ----------
	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	// ----------
	// For each object
	// - send data to shader variables.
	// - copy all buffer data.
	for (int i = 0; i < gameEntityCount; i++)
	{
		pixelShader->SetData(
			"light1",
			&directionalLight1,
			sizeof(DirectionalLight)
		);

		pixelShader->SetData(
			"light2",
			&directionalLight2,
			sizeof(DirectionalLight)
		);
		
		// For each frame.
		gameEntities[i]->PrepareMaterial(camera.GetViewMatrix(), camera.GetProjectionMatrix());


		// - set matrices.
		// vertexShader->SetMatrix4x4("world", entityWorldMatrix);
		// vertexShader->SetMatrix4x4("view", camera.GetViewMatrix());
		// vertexShader->SetMatrix4x4("projection", camera.GetProjectionMatrix());

		// Send buffer data to the vertex shader.
		// vertexShader->CopyAllBufferData();

		// Send buffer data to the vertex shader.
		// vertexShader->CopyAllBufferData();

		// Set the vertex and pixel shaders to use for the next Draw() command
		//  - These don't technically need to be set every frame...YET
		//  - Once you start applying different shaders to different objects,
		//    you'll need to swap the current shaders before each draw
		// vertexShader->SetShader();
		// pixelShader->SetShader();
		
		// Get reference to the bufferMesh.
		pSharedMesh bufferMesh = gameEntities[i]->GetMesh();

		// Collect the buffers from the meshes.
		ID3D11Buffer* buffers[2] = {
			bufferMesh->GetVertexBuffer(),
			bufferMesh->GetIndexBuffer()
		};

		UINT stride = sizeof(Vertex); // Question for Professor: Where would stride and offset be tied to? Is it input alongside the Vertex data?
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, &buffers[0], &stride, &offset);
		context->IASetIndexBuffer(buffers[1], DXGI_FORMAT_R32_UINT, 0);

		context->DrawIndexed(
			bufferMesh->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
			0,     // Offset to the first index we want to use
			0	   // Offset to add to each index when looking up vertices
		);

	}

	// End of object loops.

	// Present the back buffer to the user
	//  - Puts the final frame we're drawing into the window so the user can see it
	//  - Do this exactly ONCE PER FRAME (always at the very end of the frame)
	swapChain->Present(0, 0);
}


#pragma region Mouse Input

// --------------------------------------------------------
// Helper method for mouse clicking.  We get this information
// from the OS-level messages anyway, so these helpers have
// been created to provide basic mouse input if you want it.
// --------------------------------------------------------
void Game::OnMouseDown(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...
	camera.UpdateMouse((buttonState & 0x0001), (float)x, (float)y);

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;

	// Capture the mouse so we keep getting mouse move
	// events even if the mouse leaves the window.  we'll be
	// releasing the capture once a mouse button is released
	SetCapture(hWnd);
}

// --------------------------------------------------------
// Helper method for mouse release
// --------------------------------------------------------
void Game::OnMouseUp(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...
	camera.UpdateMouse((buttonState & 0x0001), (float)x, (float)y);

	// We don't care about the tracking the cursor outside
	// the window anymore (we're not dragging if the mouse is up)
	ReleaseCapture();
}

// --------------------------------------------------------
// Helper method for mouse movement.  We only get this message
// if the mouse is currently over the window, or if we're
// currently capturing the mouse.
// --------------------------------------------------------
void Game::OnMouseMove(WPARAM buttonState, int x, int y)
{
	// Add any custom code here...
	camera.UpdateMouse((buttonState & 0x0001), (float)x, (float)y);

	MouseTracker trackerWatch = camera.GetMouseTracker();
	/* // if (trackerWatch.GetCurrentButtonState())
	// {
		printf("\n------------\n");
		printf("Tracker Watch \n");
		printf("|  Previous State [%s] \n", (trackerWatch.GetPreviousButtonState() ? "true" : "false"));
		printf("|  Current State [%s] \n", (trackerWatch.GetCurrentButtonState() ? "true" : "false"));
		printf("|  Previous Position [< %4.5f, %4.5f >] \n", trackerWatch.GetPreviousMousePosition().x, trackerWatch.GetPreviousMousePosition().y);
		printf("|  Current Position [< %4.5f, %4.5f >] \n", trackerWatch.GetCurrentMousePosition().x, trackerWatch.GetCurrentMousePosition().y);
		printf("|  Raw Delta [< %4.5f, %4.5f >] \n", trackerWatch.GetRawDelta().x, trackerWatch.GetRawDelta().y);
		printf("|  Delta [< %4.5f, %4.5f >] \n", trackerWatch.GetDelta(XMFLOAT2(0.0f, 0.0f), XMFLOAT2(6.0f, 6.0f)).x, trackerWatch.GetDelta(XMFLOAT2(0.0f, 0.0f), XMFLOAT2(6.0f, 6.0f)).y);
		printf("------------\n");
	// } */

	if (trackerWatch.GetCurrentButtonState()) 
	{
		// Get rotation.
		float scale = 0.0025f;
		XMFLOAT2 delta = trackerWatch.GetRawDelta();
		XMFLOAT3 rotation = XMFLOAT3(delta.x * scale, delta.y * scale, 0.0f);

		// Apply rotation.
		camera.UpdateRotation(rotation);
	}

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;
}

// --------------------------------------------------------
// Helper method for mouse wheel scrolling.
// WheelDelta may be positive or negative, depending
// on the direction of the scroll
// --------------------------------------------------------
void Game::OnMouseWheel(float wheelDelta, int x, int y)
{
	// Add any custom code here...
}
#pragma endregion