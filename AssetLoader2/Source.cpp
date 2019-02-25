#include <assimp/Importer.hpp>      // C++ importer interface
#include <assimp/scene.h>           // Output data structure
#include <assimp/postprocess.h>     // Post processing flags
#include <vector>
#include "AssetPool.h"

struct Vector3
{
	Vector3() { x = 0; y = 0; z = 0; }
	Vector3(aiVector3D vec) { x = vec.x; y = vec.y; z = vec.z; }
	float x, y, z;
};

struct Mesh
{
	std::vector<unsigned int> indices;
	std::vector<Vector3> vertices, normals;
};

bool ImportModelAsset(const std::string& pFile)
{
	// Create an instance of the Importer class
	Assimp::Importer importer;
	// And have it read the given file with some example postprocessing
	// Usually - if speed is not the most important aspect for you - you'll 
	// propably to request more postprocessing than we do in this example.
	const aiScene* scene = importer.ReadFile(pFile,
		aiProcess_GenSmoothNormals		|
		aiProcess_MakeLeftHanded		|
		aiProcess_Triangulate			|
		aiProcess_JoinIdenticalVertices |
		aiProcess_SortByPType);

	// If the import failed, report it
	if (!scene)
	{
		// Log an error
		return false;
	}

	std::vector<unsigned int> indices;
	std::vector<aiVector3D> vertices, normals;

	if (scene->HasMeshes())
	{
		for (int i = 0; i < scene->mNumMeshes; ++i)
		{
			aiMesh* mesh = scene->mMeshes[i];

			for (int vertex = 0; vertex < mesh->mNumVertices; ++vertex)
			{
				vertices.push_back(mesh->mVertices[vertex]);
				normals.push_back(mesh->mNormals[vertex]);
			}

			if (mesh->HasFaces())
			{
				for (int f = 0; f < mesh->mNumFaces; ++f)
				{
					aiFace face = mesh->mFaces[f];
				
					for (int index = 0; index < face.mNumIndices; ++index)
					{
						indices.push_back(face.mIndices[index]);
					}
				}
			}
			else
			{
				// Log - No faces found on mesh.
			}
		}
	}
	else
	{
		// Log - No mesh found in file.
	}

	return true;
}

int main(int varc, char** varg)
{
	////bool res = false;
	////res = ImportModelAsset("Assets/Models/Cube.fbx");

	AssetFile<aiMesh> mesh1;
	mesh1.SetFile(nullptr, "Assets/Models/Cube.fbx");

	AssetPool<aiMesh> assetPool;
	int res = assetPool.AddFileToPool(mesh1);
	res = assetPool.AddFileToPool(mesh1);

	AssetFile<aiMesh> asset = assetPool.GetAssetByFilename("Assets/Models/Cube.fbx");
	int id = assetPool.GetAssetId("Assets/Models/Cube.fbx");
	asset = assetPool.GetAssetById(id);


	if (!res)
	{
		// Log - Error importing model
		return -1;
	}

	return 0;
}