/*
================================
 Model.h
 
 Created on: 11 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#pragma once
#pragma warning( disable : 4005 )

#include "mesh.h"
#include "texturearray.h"
#include "Moveable.h"

#include <d3d11.h>
#include <D3DX10math.h>
#include <fstream>
#include <vector>
#include <wrl/client.h>
#include <memory>

using Microsoft::WRL::ComPtr;

/*
================================
 
 Model

 A generic 3D object. Stores geometry information.
 
================================
*/
class Model : public Moveable
{
public:
	Model(void);	
	virtual ~Model(void);

	bool init(ID3D11Device* device, const std::string& meshFilename, const std::string& textureFilename, const std::string& bumpMapFilename, const std::string& specularMapFilename);
	void render(ID3D11DeviceContext* deviceContext);

	int getIndexCount() const;

	const std::vector<ID3D11ShaderResourceView*>& getTextureArray() const;

private:
	ComPtr<ID3D11Buffer> m_vertexBuffer;
	ComPtr<ID3D11Buffer> m_indexBuffer;
	int m_vertexCount;
	int m_indexCount;
	std::unique_ptr<TextureArray> m_textureArray;	
	std::vector<Mesh> m_mesh;

	bool initBuffers(ID3D11Device* device);
	void renderBuffers(ID3D11DeviceContext* deviceContext);	
	bool loadTextures(ID3D11Device* device, const std::string& filename1, const std::string& filename2, const std::string& filename3);
	bool loadMesh(const std::string& filename);
	bool loadMeshFromObj(const std::string& filename);
			
	struct Vertex
	{
		D3DXVECTOR3 position;
		D3DXVECTOR2 texture;
		D3DXVECTOR3 normal;
		D3DXVECTOR3 tangent;
		D3DXVECTOR3 bitangent;
	};

	struct TempVertex
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};
	
	void CalculateTBNVectors();	
};

