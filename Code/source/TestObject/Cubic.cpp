#include "TestObject/Cubic.h"

#include "stb_image.h"

#include "Model.h"
#include "Mesh.h"
#include "Shader.h"

#include "Component/RenderComponent.h"

static std::vector<Vertex> set_cubic_vertices();
static std::vector<unsigned int> set_cubic_indices();
static std::vector<Texture> set_cubic_textures();
static unsigned int texture_from_file(std::string filename);

Cubic::Cubic(std::string name, bool active) : GameObject(name, active)
{
	initialize_cubic();
}

void Cubic::initialize_cubic()
{
	static std::vector<Vertex> vertices = set_cubic_vertices();
	static std::vector<unsigned int> indices = set_cubic_indices();
	static std::vector<Texture> textures = set_cubic_textures();
	static Model cubic_model = Model("cubic_mode", vertices, indices, textures);
	static const char* vs_path = "opengl_project/Code/shader/vertex_shader.vs";
	static const char* fs_path = "opengl_project/Code/shader/frag_shader.fs";

	renderer->initialize_renderer(cubic_model, vs_path, fs_path);
}

static std::vector<Vertex> set_cubic_vertices()
{
	int vertex_num = 24;
	int vertex_size = 8;

	std::vector<Vertex> vertices;

	float vertex_data[] = {
	-0.5f, -0.5f, -0.5f, 0.0, 0.0, -1.0,  0.0f, 0.0f,
	 0.5f, -0.5f, -0.5f, 0.0, 0.0, -1.0,  1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f, 0.0, 0.0, -1.0,  1.0f, 1.0f,
	-0.5f,  0.5f, -0.5f, 0.0, 0.0, -1.0,  0.0f, 1.0f,

	-0.5f, -0.5f,  0.5f, 0.0, 0.0, 1.0,  0.0f, 0.0f,
	 0.5f, -0.5f,  0.5f, 0.0, 0.0, 1.0,  1.0f, 0.0f,
	 0.5f,  0.5f,  0.5f, 0.0, 0.0, 1.0,  1.0f, 1.0f,
	-0.5f,  0.5f,  0.5f, 0.0, 0.0, 1.0,  0.0f, 1.0f,

	-0.5f,  0.5f,  0.5f, -1.0, 0.0, 0.0,  1.0f, 0.0f,
	-0.5f,  0.5f, -0.5f, -1.0, 0.0, 0.0, 1.0f, 1.0f,
	-0.5f, -0.5f, -0.5f, -1.0, 0.0, 0.0, 0.0f, 1.0f,
	-0.5f, -0.5f,  0.5f, -1.0, 0.0, 0.0, 0.0f, 0.0f,

	 0.5f,  0.5f,  0.5f, 1.0, 0.0, 0.0, 1.0f, 0.0f,
	 0.5f,  0.5f, -0.5f, 1.0, 0.0, 0.0, 1.0f, 1.0f,
	 0.5f, -0.5f, -0.5f, 1.0, 0.0, 0.0, 0.0f, 1.0f,
	 0.5f, -0.5f,  0.5f, 1.0, 0.0, 0.0, 0.0f, 0.0f,

	-0.5f, -0.5f, -0.5f, 0.0, -1.0, 0.0, 0.0f, 1.0f,
	 0.5f, -0.5f, -0.5f, 0.0, -1.0, 0.0, 1.0f, 1.0f,
	 0.5f, -0.5f,  0.5f, 0.0, -1.0, 0.0, 1.0f, 0.0f,
	-0.5f, -0.5f,  0.5f, 0.0, -1.0, 0.0, 0.0f, 0.0f,

	-0.5f,  0.5f, -0.5f, 0.0, 1.0, 0.0, 0.0f, 1.0f,
	 0.5f,  0.5f, -0.5f, 0.0, 1.0, 0.0, 1.0f, 1.0f,
	 0.5f,  0.5f,  0.5f, 0.0, 1.0, 0.0, 1.0f, 0.0f,
	-0.5f,  0.5f,  0.5f, 0.0, 1.0, 0.0, 0.0f, 0.0f
	};

	for (int i = 0; i < vertex_num; ++i)
	{
		Vertex vertex;
		int index = i * vertex_size;
		vertex.Position = glm::vec3(vertex_data[index + 0], vertex_data[index + 1], vertex_data[index + 2]);
		vertex.Normal = glm::vec3(vertex_data[index + 3], vertex_data[index + 4], vertex_data[index + 5]);
		vertex.UV = glm::vec2(vertex_data[index + 6], vertex_data[index + 7]);

		vertices.push_back(vertex);
	}

	return vertices;
}

static std::vector<unsigned int> set_cubic_indices()
{
	unsigned int indices_data[] = {
	0, 1, 2,
	0, 2, 3,

	4, 5, 6,
	4, 6, 7,

	8, 9, 10,
	8, 10, 11,

	12, 13, 14,
	12, 14, 15,

	16, 17, 18,
	16, 18, 19,

	20, 21, 22,
	20, 22, 23
	};

	return std::vector<unsigned int> (indices_data, indices_data + 36);
}

static std::vector<Texture> set_cubic_textures()
{
	std::vector<Texture> textures;

	int texture_num = 2;
	std::string texture_path[] = { "opengl_project/Resource/test_texture.jpg", "opengl_project/Resource/test_specular_texture.jpg" };
	Texture_Type texture_type [] = { DIFFUSE_TEXT, SPECULAR_TEXT };
	
	for (int i = 0; i < texture_num; ++i)
	{
		Texture texture;

		texture.id = texture_from_file( texture_path[i] );
		texture.path_hash = std::hash<std::string>() (texture_path[i]);
		texture.type = texture_type[i];
		
		textures.push_back(texture);
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