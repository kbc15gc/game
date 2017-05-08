#include "Material.h"

Material::Material(char* name):
	_Name(name)
{
	_BlendColor = Color::white;
	FOR(i, TextureHandleE::TextureNum)
	{
		_Textures[i] = nullptr;
	}
}

Material::~Material()
{
	FOR(i, TextureHandleE::TextureNum)
	{

		SAFE_DELETE(_Textures[i]);
	}
}

void Material::SetTexture(const UINT & handle, IDirect3DBaseTexture9 * tex)
{
	_Textures[handle] = tex;
}

IDirect3DBaseTexture9 * Material::GetTexture(const UINT & handle)
{
	return _Textures[handle];
}

void Material::SetColor(Color color)
{
	_BlendColor = color;
}

const Color & Material::GetBlendColor()
{
	return _BlendColor;
}

void Material::SetName(char * name)
{
	_Name = name;
}

const char * Material::GetName()
{
	return _Name;
}
