#include "Model.h"

#include <iostream>

Model::Model(const std::string path)
{
	load_mesh(path);
}

void Model::Draw(Shader& shader)
{
	for (auto&& mesh : meshes)
	{
		mesh.Draw(shader);
	}
}

void Model::load_mesh(const std::string path)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(path, 
							aiProcess_Triangulate | 
							aiProcess_GenSmoothNormals | 
							aiProcess_FlipUVs | 
							aiProcess_CalcTangentSpace);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !(scene->mRootNode))
	{
		std::cout << "ERROR::ASSIMP:: " << importer.GetErrorString() << std::endl;
		return;
	}

	process_node(scene->mRootNode, scene);
}

void Model::process_node(aiNode* node, const aiScene* scene)
{
	for (int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		meshes.push_back(process_mesh( mesh, scene ));
	}

	for (int i = 0; i < node->mNumChildren; ++i)
	{
		aiNode* child = node->mChildren[i];
		process_node(child, scene);
	}
}

Mesh Model::process_mesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector< Vertex > vertices;
	std::vector< unsigned int > indices;
	std::vector< Texture > textures;
	
	// vertices part
	for (int i = 0; i < mesh->mNumVertices; ++i)
	{
		Vertex vertex;
		
		vertex.Position = glm::vec3(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
		
		if (mesh->HasNormals())
		{
			vertex.Normal = glm::vec3(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
		}
		else
		{
			vertex.Normal = glm::vec3(0.0);
		}

		if (mesh->mTextureCoords[0])
		{
			vertex.UV = glm::vec2(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
			vertex.Tangent = glm::vec3(mesh->mTangents[i].x, mesh->mTangents[i].y, mesh->mTangents[i].z);
			vertex.Bitangent = glm::vec3(mesh->mBitangents[i].x, mesh->mBitangents[i].y, mesh->mBitangents[i].z);
		}
		else
		{
			vertex.UV = glm::vec2(0.0);
			vertex.Tangent = glm::vec3(0.0);
			vertex.Bitangent = glm::vec3(0.0);
		}

		vertices.push_back(vertex);
	}

	// indices part
	for (int i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];

		for (int j = 0; j < face.mNumIndices; ++j)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	// textures part
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];

	loadMaterialTextures(textures, material);

	return Mesh(vertices, indices, textures);
}

void Model::loadMaterialTextures(std::vector<Texture>& textures, aiMaterial* material)
{
	const static aiTextureType ai_types[] = {
		aiTextureType_DIFFUSE,
		aiTextureType_SPECULAR,
		aiTextureType_NORMALS,
		aiTextureType_HEIGHT
	};

	const static Texture_Type text_types[] = {
		DIFFUSE_TEXT,
		SPECULAR_TEXT,
		NORMAL_TEXT,
		HEIGHT_TEXT
	};

	for (int i = 0; i < sizeof(ai_types); ++i)
	{
		if (material->GetTextureCount(ai_types[i]))
		{
			Texture texture = ////////////////////////
		}


	}

}

static unsigned int load_texture_from()
{

}
