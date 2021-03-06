#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <string>

#include "Mesh.h"

class Model {
public:
	std::size_t model_hash;

	Model(const std::string path);
	Model(const std::string name, std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures);
	virtual ~Model();

	void Draw(Shader* shader);

private:
	std::vector< Mesh > meshes;
	std::vector<Texture> loaded_textures;
	std::string dictionary;

	void load_mesh(const std::string path);
	void process_node(aiNode * node, const aiScene * scene);
	void process_mesh(aiMesh* mesh, const aiScene* scene);

	std::vector<Texture> loadMaterialTextures( aiMaterial* material);
	// suppose there is only one texture for each texture_type
	//std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType ai_type, Texture_Type texture_type);
};