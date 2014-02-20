/*
================================
 texture.cpp

 Created on: 12 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#include "texturearray.h"

/*
================
 Texture::Texture
================
*/
TextureArray::TextureArray(void)	
{
	
}

/*
================
 Texture::~Texture
================
*/
TextureArray::~TextureArray(void)
{
	for ( auto texture : m_textures )
	{
		texture->Release();
	}
	m_textures.clear();
}

/*
================
 Texture::init
================
*/
bool TextureArray::init( ID3D11Device* device, const std::string& filename1, const std::string& filename2, const std::string& filename3 )
{
	m_textures.resize(3);
	
	HRESULT result;
			
	result = D3DX11CreateShaderResourceViewFromFileA(device, filename1.c_str(), NULL, NULL, &m_textures[0], NULL);		
	if (FAILED(result))
	{
		return false;
	}

	result = D3DX11CreateShaderResourceViewFromFileA(device, filename2.c_str(), NULL, NULL, &m_textures[1], NULL);		
	if (FAILED(result))
	{
		return false;
	}

	result = D3DX11CreateShaderResourceViewFromFileA(device, filename3.c_str(), NULL, NULL, &m_textures[2], NULL);		
	if (FAILED(result))
	{
		return false;
	}

	return true;
}

/*
================
 Texture::getTexture
================
*/
const std::vector<ID3D11ShaderResourceView*>& TextureArray::getTextureArray() const
{
	return m_textures;
}
