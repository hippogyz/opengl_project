#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>
#include <memory>

struct Vertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 UV;
	glm::vec3 Tangent;
	glm::vec3 Bitangent;
};

enum Texture_Type { DIFFUSE, SPECULAR, NORMAL, HEIGHT };

struct Texture {
	unsigned int id;
	Texture_Type type;
	std::size_t path_hash;
};

class Shader;

class Mesh {
public:
	std::vector< Vertex > vertices;
	std::vector< unsigned int > indices; // for EBO
	std::vector<Texture> textures;
	unsigned int VAO;

	Mesh(std::vector< Vertex > vertices, std::vector< unsigned int > indices, std::vector<Texture>textures);

	void Draw(Shader& shader);

private:
	unsigned int VBO, EBO;

	void initialize_mesh();
};