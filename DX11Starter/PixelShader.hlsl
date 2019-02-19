
// Struct representing the data we expect to receive from earlier pipeline stages
// - Should match the output of our corresponding vertex shader
// - The name of the struct itself is unimportant
// - The variable names don't have to match other shaders (just the semantics)
// - Each variable must have a semantic, which defines its usage
struct VertexToPixel
{
	// Data type
	//  |
	//  |   Name           Semantic
	//  |    |                 |
	//  v    v                 v
	float4 position		 : SV_POSITION;
	float3 normal		 : NORMAL;
	float3 worldPosition : POSITION;
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

struct PointLight 
{
	// Data type
	//  |
	//  |   Name
	//  |    |
	//  v    v
	float3 PointPosition;
	float3 PointColor;
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
	float4 SurfaceColor;
	float3 CameraPosition;
	// float SurfaceSpecularity;
};

float4 calculateLight(DirectionalLight light, float3 norm) 
{
	float3 inverseDirection = normalize(-light.Direction);
	float intensity = saturate(dot(norm, inverseDirection));
	float4 scaledDiffuse = mul(intensity, light.DiffuseColor);
	float4 lightColor = scaledDiffuse + light.AmbientColor;
	return lightColor;
}

float4 calculateDirectionalLight(
	float3 lightDirection, float3 lightColor,
	float3 surfaceNormal, float3 surfaceColor, 
	float3 cameraDirection, float surfaceShininess) {
	
	// Calculate the N dot L for lambert (diffuse) lighting.
	float3 toLight = normalize(-lightDirection);
	float dotNL = saturate(dot(toLight, surfaceNormal));

	// Calculate the specularity.
	float3 toReflection = normalize(reflect(-toLight, surfaceNormal));
	float specularity = pow(max(dot(toReflection, cameraDirection), 0), surfaceShininess);

	// Return the light.
	float3 directionalColor = (lightColor * dotNL * surfaceColor) + float3(specularity, specularity, specularity);
	return directionalColor;
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

	// Normalize the input vector after it comes from the rasterizer.
	input.normal = normalize(input.normal);

	// Get the base colors.
	float3 surfaceColor = SurfaceColor;
	float surfaceSpecularity = 64.0f; // Specularity; // Shininess of a surface. Usually passed in by a material or texture.

	// Get the direction to the cameras.
	float3 toCamera = normalize(CameraPosition - input.worldPosition);

	// Calculate the directional lights.
	float3 directionalLight1 = calculateDirectionalLight(
		light1.Direction, light1.DiffuseColor,
		input.normal, light1.AmbientColor * surfaceColor,
		toCamera, surfaceSpecularity
	);

	// Calculate the directional light.
	float3 directionalLight1 = calculateDirectionalLight(
		light2.Direction, light2.DiffuseColor,
		input.normal, light2.AmbientColor * surfaceColor,
		toCamera, surfaceSpecularity
	);

	// Calculate the point lights.


	// float4 firstLight = calculateLight(light1, input.normal);
	// float4 secondLight = calculateLight(light2, input.normal);
	// float4 finalColor = firstLight + secondLight;
	// float4 surfaceColor = normalize(finalColor) * normalize(surface);
	
	// Just return the input color
	// - This color (like most values passing through the rasterizer) is 
	//   interpolated for each pixel between the corresponding vertices 
	//   of the triangle we're rendering
	// return float4(1, 0, 0, 1);
	return normalize(surfaceColor);
}