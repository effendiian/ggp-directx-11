
// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name          Semantic
	//  |    |                |
	//  v    v                v
	float4 position		: SV_POSITION;
	float3 normal		: NORMAL;
};

struct DirectionalLight
{
	// Data type
	//  |
	//  |   Name
	//  |    |
	//  v    v
	float4 AmbientColor;
	float4 DiffuseColor;
	float3 Direction;
};

// Constant Buffer
// - Allows us to define a buffer of individual variables 
//    which will (eventually) hold data from our C++ code
// - All non-pipeline variables that get their values from 
//    our C++ code must be defined inside a Constant Buffer
// - The name of the cbuffer itself is unimportant
cbuffer externalData : register(b0)
{
	DirectionalLight light1;
	DirectionalLight light2;
	float4 surface;
};

float4 calculateLight(DirectionalLight light, float3 norm) 
{
	float3 inverseDirection = normalize(-light.Direction);
	float intensity = saturate(dot(norm, inverseDirection));
	float4 scaledDiffuse = mul(intensity, light.DiffuseColor);
	float4 lightColor = scaledDiffuse + light.AmbientColor;
	return lightColor;
}

// --------------------------------------------------------
// The entry point (main method) for our pixel shader
// 
// - Input is the data coming down the pipeline (defined by the struct)
// - Output is a single color (float4)
// - Has a special semantic (SV_TARGET), which means 
//    "put the output of this into the current render target"
// - Named "main" because that's the default the shader compiler looks for
// --------------------------------------------------------
float4 main(VertexToPixel input) : SV_TARGET
{

	// Normalize the input vector.
	input.normal = normalize(input.normal);

	float4 firstLight = calculateLight(light1, input.normal);
	float4 secondLight = calculateLight(light2, input.normal);
	float4 finalColor = firstLight + secondLight;
	float4 surfaceColor = normalize(finalColor) * normalize(surface);
	
	// Just return the input color
	// - This color (like most values passing through the rasterizer) is 
	//   interpolated for each pixel between the corresponding vertices 
	//   of the triangle we're rendering
	// return float4(1, 0, 0, 1);
	return normalize(surfaceColor);
}