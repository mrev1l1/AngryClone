#include "ModelFileHandler.h"
#include <windows.h>

ModelFileHandler::ModelFileHandler(char* fileName)
{
	void parseLine(char* line, AngryClone::VertexPositionColor* result);

	/*Windows::Storage::StorageFile^ MFile;
	auto folder = Windows::Storage::ApplicationData::Current->LocalFolder->Path + "\\cubeModel.txt";
	concurrency::task<Windows::Storage::StorageFile^> MFileReader(MFile->GetFileFromPathAsync(folder));
	MFileReader.then([this](Windows::Storage::StorageFile^ sampleFile)
	{
		Platform::String^ MFileData;
		return Windows::Storage::FileIO::WriteTextAsync(sampleFile, MFileData);
	}).then([this](concurrency::task<void> previousOperation) {
		try {
			previousOperation.get();
		}
		catch (Platform::Exception^) {
		}
	});*/


	this->ModelFile = new std::fstream(L"C:\\Users\\Олександр\\AppData\\Local\\Packages\\a5f5ffab-88e8-40fd-8e9e-80a2994ded96_kqgv1awp48cbp\\LocalState\\cubeModel.txt", std::ios_base::in);
	
	

	std::string* line;
	char buffer[64];

	if (ModelFile->is_open())
	{
		while (!ModelFile->eof())
		{
			ModelFile->getline(buffer, 64);
			line = new std::string(buffer); 

			int VerticesCount = std::stoi(*line);
			const int n = VerticesCount;
			
			this->Vertices = new AngryClone::VertexPositionColor[VerticesCount];
			
			for (int i = VerticesCount; i > 0; i--)
			{
				ModelFile->getline(buffer, 64);

				parseLine(buffer, &this->Vertices[VerticesCount - i]);

			}

			this->VerticesBufferSize = sizeof(AngryClone::VertexPositionColor) * VerticesCount;
			
			delete line;

			ModelFile->getline(buffer, 64);
			line = new std::string(buffer);

			int IndicesCount = std::stoi(*line);

			this->Indices = new unsigned short[IndicesCount];

			int j = 0;
			for (int i = IndicesCount; i > 0; i--)
			{
				ModelFile->getline(buffer, 64);
				
				char *nextToken = NULL;
				this->Indices[j] = stoi(strtok_s(buffer, ",", &nextToken));
				j++;

				int iteration = 2;
				while (iteration > 0)
				{
					this->Indices[j] = stoi(strtok_s(NULL, ",", &nextToken));
					j++;
					iteration--;
				}
			}

			this->IndicesBufferSize = sizeof(unsigned short) * IndicesCount * 3;
		}

		ModelFile->close();
	}
	else
	{
		this->Indices = nullptr;
		this->Vertices = nullptr;
	}
	
}

void parseLine(char* line, AngryClone::VertexPositionColor* result)
{
	char* nextToken = NULL;
	
	result->pos.x = std::atof(strtok_s(line, " ", &nextToken));
	result->pos.y = std::atof(strtok_s(NULL, " ", &nextToken));
	result->pos.z = std::atof(strtok_s(NULL, " ", &nextToken));
	
	result->color.x = std::atof(strtok_s(NULL, " ", &nextToken));
	result->color.y = std::atof(strtok_s(NULL, " ", &nextToken));
	result->color.z = std::atof(strtok_s(NULL, " ", &nextToken));
}

AngryClone::VertexPositionColor* ModelFileHandler::getVertices()
{
	return this->Vertices;
}

unsigned short* ModelFileHandler::getIndices()
{
	return this->Indices;
}

int ModelFileHandler::getIndicesCount()
{
	return this->IndicesBufferSize / sizeof(unsigned short);
}

long ModelFileHandler::getIndicesBufferSize()
{
	return this->IndicesBufferSize;
}

long ModelFileHandler::getVerticesBufferSize()
{
	return this->VerticesBufferSize;
}

ModelFileHandler::~ModelFileHandler()
{
	/*delete this->ModelFile;
	delete this->CubeIndices;
	delete this->Vertices;*/
}
