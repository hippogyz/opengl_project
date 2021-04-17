#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <vector>
#include <string>

#include "Mesh.h"

class Model {
public:
	Model(const std::string path);

	virtual ~Model();

	void Draw(Shader& shader);

private:
	std::vector< Mesh > meshes;
	std::vector<Texture> loaded_textures;
	std::string dictionary;

	void load_mesh(const std::string path);
	void process_node(aiNode * node, const aiScene * scene);
	Mesh process_mesh(aiMesh* mesh, const aiScene* scene);
	std::vector<Texture> loadMaterialTextures( aiMaterial* material);
	// suppose there is only one texture for each texture_type
	//std::vector<Texture> loadMaterialTextures(aiMaterial* mat, aiTextureType ai_type, Texture_Type texture_type);
};