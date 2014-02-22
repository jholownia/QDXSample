/*
================================
 Model.h

 Created on: 11 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#include "Model.h"
#include "objloader.h"

#include <iostream>
#include <string>


/*
================
 Model::Model
================
*/
Model::Model(void) :
    m_vertexBuffer (nullptr),
    m_indexBuffer  (nullptr),
    m_textureArray (nullptr)
{

}

/*
================
 Model::~Model
================
*/
Model::~Model(void)
{

}

/*
================
 Model::init

 Creates a 3d object given direct 3d device, .obj mesh filename and texture filename.
================
*/
bool Model::init( ID3D11Device* device, const std::string& meshFilename, const std::string& textureFilename, const std::string& bumpMapFilename, const std::string& specularMapFilename )
{
    bool result;

    // Load mesh
    {
        std::string s(meshFilename);

        if (s.find(".obj") == std::string::npos)
        {
            result = loadMesh(meshFilename);
        }
        else
        {
            result = loadMeshFromObj(meshFilename);
        }
    }
    if (!result)
    {
        return false;
    }

    CalculateTBNVectors();

    result = initBuffers(device);
    if (!result)
    {
        return false;
    }

    result = loadTextures(device, textureFilename, bumpMapFilename, specularMapFilename);
    if (!result)
    {
        return false;
    }

    return true;
}

/*
================
 Model::render
================
*/
void Model::render( ID3D11DeviceContext* deviceContext )
{
    renderBuffers(deviceContext);
}

/*
================
 Model::getIndexCount
================
*/
int Model::getIndexCount() const
{
    return m_indexCount;
}

/*
================
 Model::getTextureArray
================
*/
const std::vector<ID3D11ShaderResourceView*>& Model::getTextureArray() const
{
    return m_textureArray->getTextureArray();
}

/*
================
 Model::initBuffers

 Creates vertex and index buffers.
================
*/
bool Model::initBuffers( ID3D11Device* device )
{
    std::vector<Vertex> vertices(m_vertexCount);
    std::vector<UINT32> indices(m_indexCount);

    HRESULT result;

    for (int i = 0; i < m_vertexCount; i++)
    {
        vertices[i].position = D3DXVECTOR3(m_mesh[i].x, m_mesh[i].y, m_mesh[i].z);
        vertices[i].texture = D3DXVECTOR2(m_mesh[i].tu, m_mesh[i].tv);
        vertices[i].normal = D3DXVECTOR3(m_mesh[i].nx, m_mesh[i].ny, m_mesh[i].nz);
        vertices[i].tangent = D3DXVECTOR3(m_mesh[i].tx, m_mesh[i].ty, m_mesh[i].tz);
        vertices[i].bitangent = D3DXVECTOR3(m_mesh[i].bx, m_mesh[i].by, m_mesh[i].bz);

        indices[i] = i;
    }

    // Set up the description of the vertex buffer.
    D3D11_BUFFER_DESC vertexBufferDesc;
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(Vertex) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;

    // Give the subresource structure a pointer to the vertex data.
    D3D11_SUBRESOURCE_DATA vertexData;
    vertexData.pSysMem = &vertices[0];
    vertexData.SysMemPitch = 0;
    vertexData.SysMemSlicePitch = 0;

    // Now finally create the vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, m_vertexBuffer.GetAddressOf());
    if(FAILED(result))
    {
        return false;
    }

    // Set up the description of the index buffer.
    D3D11_BUFFER_DESC indexBufferDesc;
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;

    // Give the subresource structure a pointer to the index data.
    D3D11_SUBRESOURCE_DATA indexData;
    indexData.pSysMem = &indices[0];
    indexData.SysMemPitch = 0;
    indexData.SysMemSlicePitch = 0;

    // Create the index buffer.
    result = device->CreateBuffer(&indexBufferDesc, &indexData, m_indexBuffer.GetAddressOf());
    if(FAILED(result))
    {
        return false;
    }

    return true;
}

/*
================
 Model::renderBuffers
================
*/
void Model::renderBuffers( ID3D11DeviceContext* deviceContext )
{
    unsigned int stride = sizeof(Vertex);
    unsigned int offset = 0;

    deviceContext->IASetVertexBuffers(0, 1, m_vertexBuffer.GetAddressOf(), &stride, &offset);
    deviceContext->IASetIndexBuffer(m_indexBuffer.Get(), DXGI_FORMAT_R32_UINT, 0);
    deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

/*
================
 Model::loadTexture
================
*/
bool Model::loadTextures( ID3D11Device* device, const std::string& filename1, const std::string& filename2, const std::string& filename3 )
{
    m_textureArray.reset(new TextureArray);

    return m_textureArray->init(device, filename1, filename2, filename3);
}



void Model::CalculateTBNVectors()
{
    TempVertex vertex1, vertex2, vertex3;
    D3DXVECTOR3 tangent, bitangent, normal;

    int index = 0;
    int faceCount = m_vertexCount / 3;

    for (int i = 0; i < faceCount; ++i)
    {
        vertex1.x = m_mesh[index].x;
        vertex1.y = m_mesh[index].y;
        vertex1.z = m_mesh[index].z;
        vertex1.tu = m_mesh[index].tu;
        vertex1.tv = m_mesh[index].tv;
        vertex1.nx = m_mesh[index].nx;
        vertex1.ny = m_mesh[index].ny;
        vertex1.nz = m_mesh[index].nz;
        ++index;

        vertex2.x = m_mesh[index].x;
        vertex2.y = m_mesh[index].y;
        vertex2.z = m_mesh[index].z;
        vertex2.tu = m_mesh[index].tu;
        vertex2.tv = m_mesh[index].tv;
        vertex2.nx = m_mesh[index].nx;
        vertex2.ny = m_mesh[index].ny;
        vertex2.nz = m_mesh[index].nz;
        ++index;

        vertex3.x = m_mesh[index].x;
        vertex3.y = m_mesh[index].y;
        vertex3.z = m_mesh[index].z;
        vertex3.tu = m_mesh[index].tu;
        vertex3.tv = m_mesh[index].tv;
        vertex3.nx = m_mesh[index].nx;
        vertex3.ny = m_mesh[index].ny;
        vertex3.nz = m_mesh[index].nz;
        ++index;

        // Calculate the tangent and bitangent of this face
        float vector1[3], vector2[3];
        float tuVector[2], tvVector[2];

        // Edge 1
        vector1[0] = vertex2.x - vertex1.x;
        vector1[1] = vertex2.y - vertex1.y;
        vector1[2] = vertex2.z - vertex1.z;

        // Edge 2
        vector2[0] = vertex3.x - vertex1.x;
        vector2[1] = vertex3.y - vertex1.y;
        vector2[2] = vertex3.z - vertex1.z;

        tuVector[0] = vertex2.tu - vertex1.tu;
        tvVector[0] = vertex2.tv - vertex1.tv;

        tuVector[1] = vertex3.tu - vertex1.tu;
        tvVector[1] = vertex3.tv - vertex1.tv;

        // Calculate denominator for tangent equation
        float den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

        // Calculate the cross product and multiply by the coefficient to get tangent and binormal
        tangent.x = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
        tangent.y = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
        tangent.z = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

        bitangent.x = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
        bitangent.y = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
        bitangent.z = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;

        D3DXVec3Normalize(&tangent, &tangent);
        D3DXVec3Normalize(&bitangent, &bitangent);

        // Calculate new normal using tangent and binormal
        D3DXVec3Cross(&normal, &tangent, &bitangent);
        D3DXVec3Normalize(&normal, &normal);

        // Smooth the normals (dirty)
        D3DXVECTOR3 normal1, normal2, normal3, tangent1, tangent2, tangent3, bitangent1, bitangent2, bitangent3;

        normal1.x = m_mesh[index - 3].nx;
        normal1.y = m_mesh[index - 3].ny;
        normal1.z = m_mesh[index - 3].nz;

        normal2.x = m_mesh[index - 2].nx;
        normal2.y = m_mesh[index - 2].ny;
        normal2.z = m_mesh[index - 2].nz;

        normal3.x = m_mesh[index - 1].nx;
        normal3.y = m_mesh[index - 1].ny;
        normal3.z = m_mesh[index - 1].nz;

        D3DXVECTOR3 axis;
        D3DXVECTOR4 temp;
        D3DXMATRIX rotation;
        float cosAngle, angle;

        // Vertex 1
        // Find rotation axis and angle between face normal and vertex normal
        cosAngle = D3DXVec3Dot(&normal, &normal1);
        angle = acos(cosAngle);
        D3DXVec3Cross(&axis, &normal, &normal1);
        D3DXVec3Normalize(&axis, &axis);

        // Create rotation matrix
        D3DXMatrixRotationAxis(&rotation, &axis, angle);

        // Transform each of the vectors
        D3DXVec3Transform(&temp, &normal, &rotation);
        normal1.x = temp.x;
        normal1.y = temp.y;
        normal1.z = temp.z;

        D3DXVec3Transform(&temp, &tangent, &rotation);
        tangent1.x = temp.x;
        tangent1.y = temp.y;
        tangent1.z = temp.z;

        D3DXVec3Transform(&temp, &bitangent, &rotation);
        bitangent1.x = temp.x;
        bitangent1.y = temp.y;
        bitangent1.z = temp.z;

        // Vertex 2
        cosAngle = D3DXVec3Dot(&normal, &normal2);
        angle = acos(cosAngle);

        D3DXVec3Cross(&axis, &normal, &normal2);
        D3DXVec3Normalize(&axis, &axis);

        D3DXMatrixRotationAxis(&rotation, &axis, angle);

        D3DXVec3Transform(&temp, &normal, &rotation);
        normal2.x = temp.x;
        normal2.y = temp.y;
        normal2.z = temp.z;

        D3DXVec3Transform(&temp, &tangent, &rotation);
        tangent2.x = temp.x;
        tangent2.y = temp.y;
        tangent2.z = temp.z;

        D3DXVec3Transform(&temp, &bitangent, &rotation);
        bitangent2.x = temp.x;
        bitangent2.y = temp.y;
        bitangent2.z = temp.z;

        // Vertex 3
        cosAngle = D3DXVec3Dot(&normal, &normal3);
        angle = acos(cosAngle);

        D3DXVec3Cross(&axis, &normal, &normal3);
        D3DXVec3Normalize(&axis, &axis);

        D3DXMatrixRotationAxis(&rotation, &axis, angle);

        D3DXVec3Transform(&temp, &normal, &rotation);
        normal3.x = temp.x;
        normal3.y = temp.y;
        normal3.z = temp.z;

        D3DXVec3Transform(&temp, &tangent, &rotation);
        tangent3.x = temp.x;
        tangent3.y = temp.y;
        tangent3.z = temp.z;

        D3DXVec3Transform(&temp, &bitangent, &rotation);
        bitangent3.x = temp.x;
        bitangent3.y = temp.y;
        bitangent3.z = temp.z;

        D3DXVec3Normalize(&normal1, &normal1);
        D3DXVec3Normalize(&normal2, &normal2);
        D3DXVec3Normalize(&normal3, &normal3);
        D3DXVec3Normalize(&tangent1, &tangent1);
        D3DXVec3Normalize(&tangent2, &tangent2);
        D3DXVec3Normalize(&tangent3, &tangent3);
        D3DXVec3Normalize(&bitangent1, &bitangent1);
        D3DXVec3Normalize(&bitangent2, &bitangent2);
        D3DXVec3Normalize(&bitangent3, &bitangent3);

        // Store the results back in mesh
        m_mesh[index - 1].nx = normal3.x;
        m_mesh[index - 1].ny = normal3.y;
        m_mesh[index - 1].nz = normal3.z;
        m_mesh[index - 1].tx = tangent3.x;
        m_mesh[index - 1].ty = tangent3.y;
        m_mesh[index - 1].tz = tangent3.z;
        m_mesh[index - 1].bx = bitangent3.x;
        m_mesh[index - 1].by = bitangent3.y;
        m_mesh[index - 1].bz = bitangent3.z;

        m_mesh[index - 2].nx = normal2.x;
        m_mesh[index - 2].ny = normal2.y;
        m_mesh[index - 2].nz = normal2.z;
        m_mesh[index - 2].tx = tangent2.x;
        m_mesh[index - 2].ty = tangent2.y;
        m_mesh[index - 2].tz = tangent2.z;
        m_mesh[index - 2].bx = bitangent2.x;
        m_mesh[index - 2].by = bitangent2.y;
        m_mesh[index - 2].bz = bitangent2.z;

        m_mesh[index - 3].nx = normal1.x;
        m_mesh[index - 3].ny = normal1.y;
        m_mesh[index - 3].nz = normal1.z;
        m_mesh[index - 3].tx = tangent1.x;
        m_mesh[index - 3].ty = tangent1.y;
        m_mesh[index - 3].tz = tangent1.z;
        m_mesh[index - 3].bx = bitangent1.x;
        m_mesh[index - 3].by = bitangent1.y;
        m_mesh[index - 3].bz = bitangent1.z;
    }
}

/*
================
 Model::loadMesh

 Loads mesh data from a simple text file with
 the following format:

 Vertex Count: nnn

 Data:
 x, y, z, tu, tv, nx, ny, nz
 x, y, z, tu, tv, nx, ny, nz
 ...
================
*/
bool Model::loadMesh( const std::string& filename )
{
    std::ifstream ifs;

    ifs.open(filename);
    if (ifs.fail())
    {
        return false;
    }

    char input;

    // Get to vertex count
    ifs.get(input);
    while(input != ':')
    {
        ifs.get(input);
    }

    // read vertex count
    ifs >> m_vertexCount;
    m_indexCount = m_vertexCount;

    // Create mesh
    m_mesh.resize(m_vertexCount);

    // Read to the beginning of the data
    ifs.get(input);
    while(input != ':')
    {
        ifs.get(input);
    }
    ifs.get(input);
    ifs.get(input);

    // Read the data
    for (int i = 0; i < m_vertexCount; ++i)
    {
        ifs >> m_mesh[i].x >> m_mesh[i].y >> m_mesh[i].z;
        ifs >> m_mesh[i].tu >> m_mesh[i].tv;
        ifs >> m_mesh[i].nx >> m_mesh[i].ny >> m_mesh[i].nz;
    }

    ifs.close();

    return true;
}

/*
================
 Model::loadMeshFromObj
================
*/
bool Model::loadMeshFromObj( const std::string& filename )
{
    bool result;

    ObjLoader loader;
    result = loader.init(filename);
    loader.createMesh(m_vertexCount, m_mesh);

    m_indexCount = m_vertexCount;

// 	// Save mesh (for debugging)
// 	std::ofstream ofs;
// 	ofs.open("sphere.txt");
//
// 	ofs << "Vertex Count: " << m_vertexCount;
// 	ofs << "\n\n";
// 	ofs << "Data:\n\n";
//
// 	for (Mesh m : m_mesh)
// 	{
// 		ofs << m.x << " " << m.y << " " << m.z << " " << m.tu << " " << m.tv << " " << m.nx << " " << m.ny << " " << m.nz << "\n";
// 	}
//
// 	ofs.close();

    return result;
}

