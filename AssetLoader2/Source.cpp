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

bool ImportModelAsset(const std::string& pFile, Mesh* out)
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
	std::vector<Vector3> vertices, normals;

	if (scene->HasMeshes())
	{
		for (int i = 0; i < scene->mNumMeshes; ++i)
		{
			aiMesh* mesh = scene->mMeshes[i];

			for (int vertex = 0; vertex < mesh->mNumVertices; ++vertex)
			{
				vertices.push_back(mesh->mVertices[vertex]);
				vertices.push_back(mesh->mNormals[vertex]);
				
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

	out->indices = indices;
	out->vertices = vertices;
	out->normals = normals;

	return true;
}

int main(int varc, char** varg)
{
	bool res = false;
	Mesh* mesh = new Mesh();

	AssetFile<Mesh> mesh1;
	res = ImportModelAsset("Assets/Models/Cube.fbx", mesh);
	mesh1.SetFile(mesh, "Assets/Models/Cube.fbx");

	AssetPool<Mesh> assetPool;
	////res = assetPool.AddFileToPool(mesh1);
	////res = assetPool.AddFileToPool(mesh1);

	////AssetFile<Mesh> asset = assetPool.GetAssetByFilename("Assets/Models/Cube.fbx");
	////int id = assetPool.GetAssetId("Assets/Models/Cube.fbx");
	////asset = assetPool.GetAssetById(id);

	assetPool.AddFilesToPool(mesh1);
	const unsigned short assetId = assetPool.GetAssetId(mesh1.GetFileName());
	assetPool.RemoveFilesFromPool(assetId);

	if (!res)
	{
		// Log - Error importing model
		return -1;
	}

	delete mesh;

	return 0;
}