/*
================================
 objloader.h

 Created on: 14 Dec 2012
 Author: Jan Holownia

 Copyright (c) Jan Holownia <jan.holownia@gmail.com> 2012.
================================
*/

#pragma once

#include "mesh.h"

#include <string>
#include <vector>
#include <memory>

/*
================================
 
 ObjLoader

 A simple loader for .OBJ files.
 
================================
*/
class ObjLoader
{
public:
	ObjLoader();
	~ObjLoader();

	bool init(std::string filename);

	void createMesh(int& vertexCount, std::vector<Mesh>& mesh);
	
private:
	struct Vector
	{
		float x, y, z;
	};

	struct Face
	{
		int vIndex1, vIndex2, vIndex3;
		int tIndex1, tIndex2, tIndex3;
		int nIndex1, nIndex2, nIndex3;
	};

	std::unique_ptr<Vector[]> m_vertices;
	std::unique_ptr<Vector[]> m_texcoords;
	std::unique_ptr<Vector[]> m_normals;
	std::unique_ptr<Face[]> m_faces;

	int m_vertexIndex;
	int m_texcoordIndex;
	int m_normalIndex;
	int m_faceIndex;
	
	bool readFileCounts(std::string, int&, int&, int&, int&);
	bool loadDataStructures(std::string, int, int, int, int);
};

