/*
================================
 texture.h

 Created on: 12 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#pragma once
#pragma warning( disable : 4005 )

#include <D3D11.h>
#include <D3DX11.h>
#include <string>
#include <vector>


/*
================================
 
 Texture

 A simple wrapper for texture.
 
================================
*/
class TextureArray
{
public:
	TextureArray(void);
	~TextureArray(void);

	bool init(ID3D11Device* device, const std::string& filename1, const std::string& filename2, const std::string& filename3);
	
	const std::vector<ID3D11ShaderResourceView*>& getTextureArray() const;

private:
	std::vector<ID3D11ShaderResourceView*> m_textures;
};

