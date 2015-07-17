#pragma once

#include "pch.h"
#include <iostream>
#include <fstream>
#include <string>
#include <cstring>

using namespace AngryClone;

class ModelFileHandler
{
	std::fstream* ModelFile;
	AngryClone::VertexPositionColor* Vertices;
	long VerticesBufferSize;
	unsigned short* Indices;
	long IndicesBufferSize;
public:
	ModelFileHandler(char* fileName);
	~ModelFileHandler();

	AngryClone::VertexPositionColor* getVertices();
	unsigned short* getIndices();
	long getIndicesBufferSize();
	long getVerticesBufferSize();
	int getIndicesCount();
};

