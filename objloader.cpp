/*
================================
 objloader.cpp

 Created on: 14 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#include "objloader.h"

#include <fstream>


/*
================
 ObjLoader::ObjLoader
================
*/
ObjLoader::ObjLoader() :
	m_vertices      (nullptr),
	m_texcoords     (nullptr),
	m_normals       (nullptr),
	m_faces         (nullptr),
	m_vertexIndex   (0),
	m_texcoordIndex (0),
	m_normalIndex   (0),
	m_faceIndex     (0)
{

}

/*
================
 ObjLoader::~ObjLoader
================
*/
ObjLoader::~ObjLoader()
{

}

/*
================
 ObjLoader::init
================
*/
bool ObjLoader::init( std::string filename )
{
	int vertexCount = 0;
	int textureCount = 0;
	int normalCount = 0;
	int faceCount = 0;	

	bool result = readFileCounts(filename, vertexCount, textureCount, normalCount, faceCount);
	if (!result)
	{
		return false;
	}

	result = loadDataStructures(filename, vertexCount, textureCount, normalCount, faceCount);
	if (!result)
	{
		return false;
	}

	return true;	
}

/*
================
 ObjLoader::readFileCounts

 Goes through the file and counts the number of vertices, textures, normals and faces
 which are then used to initialize buffers properly.
================
*/
bool ObjLoader::readFileCounts( std::string filename, int& vertexCount, int& textureCount, int& normalCount, int& faceCount )
{
	std::ifstream ifs;
	char input;

	ifs.open(filename);
	if (ifs.fail())
	{
		return false;
	}

	// Read the whole file and count the stuff
	ifs.get(input);
	while (!ifs.eof())
	{
		// If the line starts with 'v' then count either the vertex, the texture coordinates, or the normal vector.
		if (input == 'v')
		{
			ifs.get(input);
			
			if (input == ' ')
			{
				++vertexCount;
			}
			else if (input == 't')
			{
				++textureCount;
			}
			else if (input == 'n')
			{
				++normalCount;
			}
		}
		// If the line starts with 'f' then increment the face count.
		if(input == 'f')
		{
			ifs.get(input);

			if(input == ' ') 
			{ 
				++faceCount; 
			}
		}

		// Otherwise read in the remainder of the line.
		while(input != '\n')
		{
			ifs.get(input);
		}

		// Start reading the beginning of the next line.
		ifs.get(input);
	}

	// Close the file.
	ifs.close();

	return true;
}

/*
================
 ObjLoader::loadDataStructures

 Loads data into buffers.
================
*/
bool ObjLoader::loadDataStructures( std::string filename, int vertexCount, int textureCount, int normalCount, int faceCount )
{
	m_vertices.reset(new Vector[vertexCount]);
	m_texcoords.reset(new Vector[textureCount]);
	m_normals.reset(new Vector[normalCount]);
	m_faces.reset(new Face[faceCount]);

	char input;
	char input2;
	
	std::ifstream ifs;

	ifs.open(filename);
	if (ifs.fail())
	{
		return false;
	}

	// Read the data in and convert RH coordinates to LH
	ifs.get(input);
	while (!ifs.eof())
	{
		if(input == 'v')
		{
			ifs.get(input);

			// Read in the vertices.
			if(input == ' ') 
			{ 
				ifs >> m_vertices[m_vertexIndex].x >> m_vertices[m_vertexIndex].y >> m_vertices[m_vertexIndex].z;

				// Invert the Z vertex to change to left hand system.
				m_vertices[m_vertexIndex].z = m_vertices[m_vertexIndex].z * -1.0f;
				m_vertexIndex++; 
			}

			// Read in the texture uv coordinates.
			if(input == 't') 
			{ 
				ifs >> m_texcoords[m_texcoordIndex].x >> m_texcoords[m_texcoordIndex].y;

				// Invert the V texture coordinates to left hand system.
				m_texcoords[m_texcoordIndex].y = 1.0f - m_texcoords[m_texcoordIndex].y;
				m_texcoordIndex++; 
			}

			// Read in the normals.
			if(input == 'n') 
			{ 
				ifs >> m_normals[m_normalIndex].x >> m_normals[m_normalIndex].y >> m_normals[m_normalIndex].z;

				// Invert the Z normal to change to left hand system.
				m_normals[m_normalIndex].z = m_normals[m_normalIndex].z * -1.0f;
				m_normalIndex++; 
			}
		}

		// Read in the faces.
		if(input == 'f') 
		{
			ifs.get(input);
			if(input == ' ')
			{
				// Read the face data in backwards to convert it to a left hand system from right hand system.
				ifs >> m_faces[m_faceIndex].vIndex3 >> input2 >> m_faces[m_faceIndex].tIndex3 >> input2 >> m_faces[m_faceIndex].nIndex3
					>> m_faces[m_faceIndex].vIndex2 >> input2 >> m_faces[m_faceIndex].tIndex2 >> input2 >> m_faces[m_faceIndex].nIndex2
					>> m_faces[m_faceIndex].vIndex1 >> input2 >> m_faces[m_faceIndex].tIndex1 >> input2 >> m_faces[m_faceIndex].nIndex1;
				m_faceIndex++;
			}
		}

		// Read in the remainder of the line.
		while(input != '\n')
		{
			ifs.get(input);
		}

		// Start reading the beginning of the next line.
		ifs.get(input);
	}

	ifs.close();

	return true;
}

/*
================
 ObjLoader::createMesh
================
*/
void ObjLoader::createMesh( int& vertexCount, std::vector<Mesh>& mesh )
{
	vertexCount = m_faceIndex * 3;
	mesh.resize(vertexCount);

	int vIndex, tIndex, nIndex;
	int m = 0;

	for (int i = 0; i < m_faceIndex; ++i)
	{
		vIndex = m_faces[i].vIndex1 - 1;
		tIndex = m_faces[i].tIndex1 - 1;
		nIndex = m_faces[i].nIndex1 - 1;

		mesh[m].x = m_vertices[vIndex].x;
		mesh[m].y = m_vertices[vIndex].y;
		mesh[m].z = m_vertices[vIndex].z;

		mesh[m].tu = m_texcoords[tIndex].x;
		mesh[m].tv = m_texcoords[tIndex].y;

		mesh[m].nx = m_normals[nIndex].x;
		mesh[m].ny = m_normals[nIndex].y;
		mesh[m].nz = m_normals[nIndex].z;

		++m;

		vIndex = m_faces[i].vIndex2 - 1;
		tIndex = m_faces[i].tIndex2 - 1;
		nIndex = m_faces[i].nIndex2 - 1;

		mesh[m].x = m_vertices[vIndex].x;
		mesh[m].y = m_vertices[vIndex].y;
		mesh[m].z = m_vertices[vIndex].z;

		mesh[m].tu = m_texcoords[tIndex].x;
		mesh[m].tv = m_texcoords[tIndex].y;

		mesh[m].nx = m_normals[nIndex].x;
		mesh[m].ny = m_normals[nIndex].y;
		mesh[m].nz = m_normals[nIndex].z;

		++m;

		vIndex = m_faces[i].vIndex3 - 1;
		tIndex = m_faces[i].tIndex3 - 1;
		nIndex = m_faces[i].nIndex3 - 1;

		mesh[m].x = m_vertices[vIndex].x;
		mesh[m].y = m_vertices[vIndex].y;
		mesh[m].z = m_vertices[vIndex].z;

		mesh[m].tu = m_texcoords[tIndex].x;
		mesh[m].tv = m_texcoords[tIndex].y;

		mesh[m].nx = m_normals[nIndex].x;
		mesh[m].ny = m_normals[nIndex].y;
		mesh[m].nz = m_normals[nIndex].z;

		++m;
	}
}
