#include "Game.h"
#include "Vertex.h"

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
		true)			// Show extra stats (fps) in title bar?
{
	// Initialize fields
	vertexShader = 0;
	pixelShader = 0;

	// Initialize the meshes.
	this->meshCount = 3;
	this->meshObjects = nullptr;

#if defined(DEBUG) || defined(_DEBUG)
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
	// Equivalent to vector_ptr->clear():
	/* for (int i = 0; i < meshCount; i++) {
		delete (*meshObjects)[i]; // delete each element in the dereferenced vector.
	}*/

	// Clean up our meshes.
	meshObjects->clear(); // Destroy each object stored in the dereferenced pointer. (However memory is still allocated).
	delete meshObjects; // Delete the vector pointer to deallocate the memory.

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
		  Vertex({ XMFLOAT3(+1.0f, -2.0f, +0.0f), maroon })},
	};


	// Set up the indices, which tell us which vertices to use and in which order
	// - This is somewhat redundant for just 3 vertices (it's a simple example)
	// - Indices are technically not required if the vertices are in the buffer
	//    in the correct order and each one will be used exactly once
	// - But just to see how it's done...
	std::vector<unsigned int> indices[] = {
		{ 0, 1, 2 },
		{ 0, 1, 2, 3, 2, 0 },
		{ 0, 1, 2, 0, 2, 3 },
	};

	// Set up the vector for the mesh objects.
	this->meshObjects = new std::vector<Mesh*>;

	// Convert vectors back into pointer array (so we don't have to rewrite the Mesh class).
	// Notes on doing this found here: https://stackoverflow.com/questions/2923272/how-to-convert-vector-to-array
	// Spec says that vectors are guaranteed to be contiguous in memory; so we can get the address to the first element.
	for (size_t i = 0; i < (size_t)meshCount; i++) {
		Vertex* v = vertices[i].data();
		unsigned int* w = indices[i].data();
		meshObjects->push_back(new Mesh(v, static_cast<unsigned int>(vertices[i].size()), w, static_cast<unsigned int>(indices[i].size()), device));
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
	if (GetAsyncKeyState(VK_ESCAPE))
		Quit();
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

	// Send data to shader variables
	//  - Do this ONCE PER OBJECT you're drawing
	//  - This is actually a complex process of copying data to a local buffer
	//    and then copying that entire buffer to the GPU.
	//  - The "SimpleShader" class handles all of that for you.
	vertexShader->SetMatrix4x4("world", worldMatrix);
	vertexShader->SetMatrix4x4("view", viewMatrix);
	vertexShader->SetMatrix4x4("projection", projectionMatrix);

	// Once you've set all of the data you care to change for
	// the next draw call, you need to actually send it to the GPU
	//  - If you skip this, the "SetMatrix" calls above won't make it to the GPU!
	vertexShader->CopyAllBufferData();

	// Set the vertex and pixel shaders to use for the next Draw() command
	//  - These don't technically need to be set every frame...YET
	//  - Once you start applying different shaders to different objects,
	//    you'll need to swap the current shaders before each draw
	vertexShader->SetShader();
	pixelShader->SetShader();

	// Loop through the meshes.
	for (int i = 0; i < meshCount; i++) {

		// Collect the buffers from the meshes.
		Mesh* mesh = (*meshObjects)[i];
		ID3D11Buffer* buffers[2] = {
			mesh->GetVertexBuffer(),
			mesh->GetIndexBuffer()
		};

		// Set buffers in the input assembler
		//  - Do this ONCE PER OBJECT you're drawing, since each object might
		//    have different geometry.
		UINT stride = sizeof(Vertex); // Question for Professor: Where would stride and offset be tied to? Is it input alongside the Vertex data?
		UINT offset = 0;
		context->IASetVertexBuffers(0, 1, &buffers[0], &stride, &offset);
		context->IASetIndexBuffer(buffers[1], DXGI_FORMAT_R32_UINT, 0);

		// Finally do the actual drawing
		//  - Do this ONCE PER OBJECT you intend to draw
		//  - This will use all of the currently set DirectX "stuff" (shaders, buffers, etc)
		//  - DrawIndexed() uses the currently set INDEX BUFFER to look up corresponding
		//     vertices in the currently set VERTEX BUFFER
		context->DrawIndexed(
			mesh->GetIndexCount(),     // The number of indices to use (we could draw a subset if we wanted)
			0,     // Offset to the first index we want to use
			0);    // Offset to add to each index when looking up vertices
	}


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