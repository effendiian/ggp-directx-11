#include "Game.h"
#include "Vertex.h"
#include <ctime>
#include <cstdlib>

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

	// Initialize
	worldMatrix = XMFLOAT4X4();
	viewMatrix = XMFLOAT4X4();
	projectionMatrix = XMFLOAT4X4();
	prevMousePos = POINT();

	// Initialize the meshes.
	meshCount = 3;
	entityCount = 9;

	// -----------------
	// Seed the random number generator.
	srand(static_cast<unsigned> (time(0)));

#if defined(DEBUG) || defined(_DEBUG)
	// -----------------
	// Do we want a console window?  Probably only in debug mode
	CreateConsoleWindow(500, 120, 32, 120);
	printf("Console window created successfully.  Feel free to printf() here.\n");
#endif

}

// --------------------------------------------------------
// Destructor - Clean up anything our game has created:
//  - Release all DirectX objects created here
//  - Delete any objects to prevent memory leaks
// --------------------------------------------------------
Game::~Game()
{
	// When the vector is a collection of pointers,
	// that are allocated with new, we need to
	// accompany each mesh with a delete call.

	// Clear each of the entity vectors.
	// for (int i = 0; i < entityCount; i++) {
	//	delete entities[i];
	// }

	// Clear the vector and then swap it to deallocate its memory.
	entities.clear();
	std::vector<std::shared_ptr<GameEntity>>().swap(entities);

	// Clear will not delete them if the contained
	// items are pointers.
	// for (int i = 0; i < meshCount; i++) {
	//	meshObjects[i].reset();
		// delete meshObjects[i]; // Delete each pointer contained in the vector.
	// }

	// Clear the vector and then swap it to deallocate its memory.
	meshObjects.clear();
	std::vector<std::shared_ptr<Mesh>>().swap(meshObjects);

	// Delete our simple shader objects, which
	// will clean up their own internal DirectX stuff
	delete vertexShader;
	delete pixelShader;
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
}



// --------------------------------------------------------
// Initializes the matrices necessary to represent our geometry's
// transformations and our 3D camera
// --------------------------------------------------------
void Game::CreateMatrices()
{
	// Set up world matrix
	// - In an actual game, each object will need one of these and they should
	//    update when/if the object moves (every frame)
	// - You'll notice a "transpose" happening below, which is redundant for
	//    an identity matrix.  This is just to show that HLSL expects a different
	//    matrix (column major vs row major) than the DirectX Math library
	XMMATRIX W = XMMatrixIdentity();
	XMStoreFloat4x4(&worldMatrix, XMMatrixTranspose(W)); // Transpose for HLSL!

	// Create the View matrix
	// - In an actual game, recreate this matrix every time the camera
	//    moves (potentially every frame)
	// - We're using the LOOK TO function, which takes the position of the
	//    camera and the direction vector along which to look (as well as "up")
	// - Another option is the LOOK AT function, to look towards a specific
	//    point in 3D space
	XMVECTOR pos = XMVectorSet(0, 0, -5, 0);
	XMVECTOR dir = XMVectorSet(0, 0, 1, 0);
	XMVECTOR up = XMVectorSet(0, 1, 0, 0);
	XMMATRIX V = XMMatrixLookToLH(
		pos,     // The position of the "camera"
		dir,     // Direction the camera is looking
		up);     // "Up" direction in 3D space (prevents roll)
	XMStoreFloat4x4(&viewMatrix, XMMatrixTranspose(V)); // Transpose for HLSL!

	// Create the Projection matrix
	// - This should match the window's aspect ratio, and also update anytime
	//    the window resizes (which is already happening in OnResize() below)
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,		// Field of View Angle
		(float)width / height,		// Aspect ratio
		0.1f,						// Near clip plane distance
		100.0f);					// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}


// --------------------------------------------------------
// Creates the geometry we're going to draw - a single triangle for now
// --------------------------------------------------------
void Game::CreateBasicGeometry()
{
	// Create some temporary variables to represent colors
	// - Not necessary, just makes things more readable
	std::vector<XMFLOAT4> colors;
	colors.push_back(XMFLOAT4(1.0f, 0.0f, 0.0f, 1.0f)); // red;
	colors.push_back(XMFLOAT4(0.0f, 1.0f, 0.0f, 1.0f)); // green;
	colors.push_back(XMFLOAT4(0.0f, 0.0f, 1.0f, 1.0f)); // blue;
	colors.push_back(XMFLOAT4(0.5f, 0.0f, 0.0f, 1.0f)); // maroon;
	colors.push_back(XMFLOAT4(0.0f, 1.0f, 1.0f, 1.0f)); // aqua;
	colors.push_back(XMFLOAT4(0.5f, 0.5f, 0.5f, 1.0f)); // silver;
	colors.push_back(XMFLOAT4(1.0f, 0.0f, 1.0f, 1.0f)); // fuchsia;
	colors.push_back(XMFLOAT4(0.5f, 0.5f, 0.0f, 1.0f)); // olive;
	colors.push_back(XMFLOAT4(1.0f, 1.0f, 1.0f, 1.0f)); // white;

	// Name colors for ease of use.
	XMFLOAT4 red = colors[0];
	XMFLOAT4 green = colors[1];
	XMFLOAT4 blue = colors[2];
	XMFLOAT4 maroon = colors[3];
	XMFLOAT4 aqua = colors[4];
	XMFLOAT4 silver = colors[5];
	XMFLOAT4 fuchsia = colors[6];
	XMFLOAT4 olive = colors[7];
	XMFLOAT4 white = colors[8];

	// Set up the vertices of the triangle we would like to draw
	// - We're going to copy this array, exactly as it exists in memory
	//    over to a DirectX-controlled data structure (the vertex buffer)
	std::vector<Vertex> vertices[] = {

		// Triangle
		// 0: (-3, -2, 0)
		// 1: (3, -2, 0)
		// 2: (0, 3, 0)
		{
			Vertex({ XMFLOAT3(-3.0f, -2.0f, +0.0f), white }),
			Vertex({ XMFLOAT3(+3.0f, -2.0f, +0.0f), silver }),
			Vertex({ XMFLOAT3(+0.0f, +3.0f, +0.0f), green }),
		},

		// Hexagon
		// 0: (-2, 5, 0)
		// 1: (2, 5, 0)
		// 2: (6, 0, 0)
		// 3: (2, -5, 0)
		// 4: (-2, -5, 0)
		// 5: (-6, 0, 0)
		{
			Vertex({ XMFLOAT3(-2.5f, +5.0f, +0.0f), red }),
			Vertex({ XMFLOAT3(+2.5f, +5.0f, +0.0f), maroon }),
			Vertex({ XMFLOAT3(+6.0f, +0.0f, +0.0f), fuchsia }),
			Vertex({ XMFLOAT3(+2.5f, -5.0f, +0.0f), green }),
			Vertex({ XMFLOAT3(-2.5f, -5.0f, +0.0f), aqua }),
			Vertex({ XMFLOAT3(-6.0f, +0.0f, +0.0f), olive }),
		},

		// Square
		// 0: (-2, 2, 0)
		// 1: (2, 2, 0)
		// 2: (2, -2, 0)
		// 3: (-2, -2, 0)
		{
			Vertex({ XMFLOAT3(-2.0f, +2.0f, +0.0f), aqua }),
			Vertex({ XMFLOAT3(+2.0f, +2.0f, +0.0f), silver }),
			Vertex({ XMFLOAT3(+2.0f, -2.0f, +0.0f), fuchsia }),
			Vertex({ XMFLOAT3(-2.0f, -2.0f, +0.0f), blue }),
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
	std::vector<unsigned int> indices[] = {
		{ 2, 1, 0,
		  0, 1, 2 },
		{ 0, 4, 5, 0, 3, 4, 0, 1, 3, 1, 2, 3,
		  3, 2, 1, 3, 1, 0, 4, 3, 0, 5, 4, 0 },
		{ 0, 1, 3, 1, 2, 3,
		  3, 2, 1, 3, 1, 0 },
	};

	// Set up the vector for the mesh objects.
	// meshObjects = std::vector<std::unique_ptr<Mesh>>();
	// meshObjects = std::vector<Mesh*>();
	meshObjects = std::vector<std::shared_ptr<Mesh>>();

	// Convert vectors back into pointer array (so we don't have to rewrite the Mesh class).
	// Notes on doing this found here: https://stackoverflow.com/questions/2923272/how-to-convert-vector-to-array
	// Spec says that vectors are guaranteed to be contiguous in memory; so we can get the address to the first element.
	for (size_t i = 0; i < (size_t)meshCount; i++) {
		Vertex* v = vertices[i].data();
		unsigned int* w = indices[i].data();
		meshObjects.push_back(std::make_shared<Mesh>(
			v, static_cast<unsigned int>(vertices[i].size()),
			w, static_cast<unsigned int>(indices[i].size()),
			device
			)
		);
	}
}

void Game::CreateEntities() {

	// Set up the vector for the game objects.
	entities = std::vector<std::shared_ptr<GameEntity>>();

	// For each mesh type, create a new entity for that given type.
	// Now, we can create the set of entities in parallel.
	for (size_t j = 0; j < (size_t)entityCount; j++) {

		// Select mesh type randomly, 0 to (meshCount - 1).
		int meshType = static_cast <unsigned int> (rand()) / (static_cast <int> (RAND_MAX / meshCount));
		entities.push_back(std::make_shared<GameEntity>(meshObjects[meshType]));
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

	// Update our projection matrix since the window size changed
	XMMATRIX P = XMMatrixPerspectiveFovLH(
		0.25f * 3.1415926535f,	// Field of View Angle
		(float)width / height,	// Aspect ratio
		0.1f,				  	// Near clip plane distance
		100.0f);			  	// Far clip plane distance
	XMStoreFloat4x4(&projectionMatrix, XMMatrixTranspose(P)); // Transpose for HLSL!
}

// --------------------------------------------------------
// Update your game here - user input, move objects, AI, etc.
// --------------------------------------------------------
void Game::Update(float deltaTime, float totalTime)
{
	// Quit if the escape key is pressed
	if (GetAsyncKeyState('W') & 0x8000) { printf("W key is down."); }
	if (GetAsyncKeyState('S') & 0x8000) { printf("S key is down."); }
	if (GetAsyncKeyState(VK_ESCAPE)) { Quit(); }

	// Update each of the entities.
	for (size_t i = 0; i < (size_t)entityCount; i++) {
		// entities[i]->SetPositionSpeed(XMFLOAT3(0.0f, 0.0f, 0.0f));
		entities[i]->Update(deltaTime, totalTime);
	}
}

// --------------------------------------------------------
// Clear the screen, redraw everything, present to the user
// --------------------------------------------------------
void Game::Draw(float deltaTime, float totalTime)
{
	// Background color (Cornflower Blue in this case) for clearing
	const float color[4] = { 0.4f, 0.6f, 0.75f, 0.0f };

	// Clear the render target and depth buffer (erases what's on the screen)
	//  - Do this ONCE PER FRAME
	//  - At the beginning of Draw (before drawing *anything*)
	context->ClearRenderTargetView(backBufferRTV, color);
	context->ClearDepthStencilView(
		depthStencilView,
		D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL,
		1.0f,
		0);

	// For each object
	// - send data to shader variables.
	// - copy all buffer data.
	for (size_t i = 0; i < (size_t)entityCount; i++) {

		// Load values.
		XMFLOAT4X4 wMatrix;		entities[i]->LoadWorldMatrix(wMatrix);
		XMFLOAT3 pos;			entities[i]->LoadPosition(pos);
		XMFLOAT3 scale;			entities[i]->LoadScale(scale);
		XMFLOAT4 rot;			entities[i]->LoadRotation(rot);

		// Set the vertex shader.
		vertexShader->SetMatrix4x4("world", wMatrix);

		// Send buffer data to the vertex shader.
		vertexShader->CopyAllBufferData();
	}

	// For each frame.
	// - Set shaders.
	vertexShader->SetMatrix4x4("view", viewMatrix);
	vertexShader->SetMatrix4x4("projection", projectionMatrix);

	// Set the vertex and pixel shaders to use for the next Draw() command
	//  - These don't technically need to be set every frame...YET
	//  - Once you start applying different shaders to different objects,
	//    you'll need to swap the current shaders before each draw
	vertexShader->SetShader();
	pixelShader->SetShader();

	// Loop through the entities.
	for (size_t i = 0; i < (size_t)entityCount; i++) {

		// Get reference to the bufferMesh.
		std::shared_ptr<Mesh> bufferMesh = entities[i]->GetMeshReference();

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

		// Send buffer data to the vertex shader.
		// vertexShader->CopyAllBufferData();
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

	// Save the previous mouse position, so we have it for the future
	prevMousePos.x = x;
	prevMousePos.y = y;

	// Caputure the mouse so we keep getting mouse move
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