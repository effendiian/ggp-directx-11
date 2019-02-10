// -----------------------------------
// Include statements.
// -----------------------------------

#include "Material.h"

// -----------------------------------
// Friend methods.
// -----------------------------------

/// <summary>
/// Swaps the specified LHS.
/// </summary>
/// <param name="lhs">The LHS.</param>
/// <param name="rhs">The RHS.</param>
void swap(Material& lhs, Material& rhs)
{
	using std::swap;

	// Swap member data.
	swap(lhs.vertexShader, rhs.vertexShader);
	swap(lhs.vertexShader, rhs.vertexShader);
}

// -----------------------------------
// Constructor(s).
// -----------------------------------

/// <summary>
/// Initializes instance of <see cref="Material"/>.
/// </summary>
Material::Material()
	: vertexShader{ nullptr },
	pixelShader{ nullptr } {}

/// <summary>
/// Initializes a new instance of the <see cref="Material"/> class.
/// </summary>
/// <param name="_vShd">The v SHD.</param>
/// <param name="_pShd">The p SHD.</param>
Material::Material(SimpleVertexShader& _vShd, SimplePixelShader& _pShd)
	: vertexShader{ &_vShd }, pixelShader{ &_pShd } {}

/// <summary>
/// Finalizes an instance of the <see cref="Material"/> class.
/// </summary>
Material::~Material() 
{
	vertexShader = nullptr;
	pixelShader = nullptr;
}

/// <summary>
/// Initializes a new instance of the <see cref="Material"/> class.
/// </summary>
/// <param name="other">The other.</param>
Material::Material(const Material& other)
{
	// Copy data members.
	vertexShader = other.vertexShader;
	pixelShader = other.pixelShader;
}

/// <summary>
/// Initializes a new instance of the <see cref="Material"/> class.
/// </summary>
/// <param name="other">The other.</param>
Material::Material(Material&& other)
	: Material() // Initialize for temporary swap.
{
	// Swap.
	swap(*this, other);
}

/// <summary>
/// Unified copy and move operator assignment.
/// </summary>
/// <param name="other">The other.</param>
/// <returns>Returns reference to copied/moved resource.</returns>
Material& Material::operator=(Material other)
{
	// Swap.
	swap(*this, other);
	return *this;
}

// -----------------------------------
// Accessors.
// -----------------------------------

/// <summary>
/// Gets the vertex shader.
/// </summary>
/// <returns>Returns simple shader.</returns>
const SimpleVertexShader& Material::GetVertexShader() const
{
	return *(this->vertexShader);
}

/// <summary>
/// Gets the pixel shader.
/// </summary>
/// <returns>Returns simple shader.</returns>
const SimplePixelShader& Material::GetPixelShader() const
{
	return *(this->pixelShader);
}

// -----------------------------------
// Mutators.
// -----------------------------------

/// <summary>
/// Sets the vertex shader.
/// </summary>
/// <param name="_vShd">The v SHD.</param>
void Material::SetVertexShader(SimpleVertexShader& _vShd)
{
	this->vertexShader = &_vShd;
}

/// <summary>
/// Sets the pixel shader.
/// </summary>
/// <param name="_pShd">The p SHD.</param>
void Material::SetPixelShader(SimplePixelShader& _pShd)
{
	this->pixelShader = &_pShd;
}