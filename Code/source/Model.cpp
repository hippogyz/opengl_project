#include "Model.h"

#include <iostream>
#include "stb_image.h"

static unsigned int texture_from_file(std::string filename);
static unsigned int texture_from_file(const char* file, std::string dictionary);

Model::Model(const std::string path)
{
	model_hash = std::hash<std::string>()(path);

	load_mesh(path);
}

Model::Model(const std::string name, std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<Texture> textures)
{
	model_hash = std::hash<std::string>()(name);

	meshes.emplace_back(vertices, indices, textures);
}

Model::~Model()
{
	for (auto&& texture : loaded_textures)
	{
		glDeleteTextures(1, &(texture.id));
	}
}

void Model::Draw(Shader* shader)
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

	dictionary = path.substr(0, path.find_last_of('/'));

	process_node(scene->mRootNode, scene);
}

void Model::process_node(aiNode* node, const aiScene* scene)
{
	for (int i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		process_mesh( mesh, scene );
	}

	for (int i = 0; i < node->mNumChildren; ++i)
	{
		aiNode* child = node->mChildren[i];
		process_node(child, scene);
	}
}

void Model::process_mesh(aiMesh* mesh, const aiScene* scene)
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

	textures = loadMaterialTextures(material);

	meshes.emplace_back(vertices, indices, textures);
}

std::vector<Texture> Model::loadMaterialTextures(aiMaterial* material)
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

	std::vector<Texture> textures;

	for (int i = 0; i < sizeof(ai_types); ++i)
	{
		if (material->GetTextureCount(ai_types[i]))
		{
			aiString temp_str;
			material->GetTexture(ai_types[i], 0, &temp_str);
			std::size_t temp_hash = std::hash<std::string>() (temp_str.C_Str());
			bool is_loaded = false;

			for (auto&& texture : loaded_textures)
			{
				if (texture.id == temp_hash)
				{
					textures.push_back(texture);
					is_loaded = true;
					break;
				}
			}

			if (!is_loaded)
			{
				Texture temp_text;
				temp_text.id = texture_from_file( temp_str.C_Str(), dictionary );
				temp_text.path_hash = std::hash<std::string>() (temp_str.C_Str());
				temp_text.type = text_types[i];
				loaded_textures.push_back(temp_text);
				textures.push_back(temp_text);
			}
		}
	}

	return textures;
}

static unsigned int texture_from_file(std::string filename)
{
	unsigned int id;

	int width, height, nrComponents;
	stbi_set_flip_vertically_on_load(true);
	unsigned char* data = stbi_load(filename.c_str(), &width, &height, &nrComponents, 0);

	if (data && strlen((char*)(data)) < 4 * 65536)
	{
		GLenum format;
		if (nrComponents == 1)
			format = GL_RED;
		else if (nrComponents == 3)
			format = GL_RGB;
		else if (nrComponents == 4)
			format = GL_RGBA;
		else
		{
			printf(" --- \"Model.cpp\" texture_from_file: nrComponents error (not RED or RGB or RGBA)--- \n");
			format = GL_RED;
		}

		glGenTextures(1, &id);
		glBindTexture(GL_TEXTURE_2D, id);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	}
	else if (!data)
	{
		printf(" --- \"Model.cpp\" texture_from_file: failed to load texture at path: %s--- \n", filename.c_str());
		id = 0;
	}
	else
	{
		printf(" --- \"Model.cpp\" texture_from_file: loaded texture at path %s--- is oversized with %d bytes \n",
			filename.c_str(), int(strlen((char*)(data))));
		id = 0;
	}
	stbi_image_free(data);

	glBindTexture(GL_TEXTURE_2D, 0);
	return id;
}

static unsigned int texture_from_file(const char* file, std::string dictionary)
{
	std::string filename = dictionary + '/' + file;
	return texture_from_file(filename);
}
