#pragma once

#include "GameObject.h"
#include "Model.h"

class Cubic : public GameObject
{
public:
	Cubic(std::string name, bool active = true);
	Cubic(glm::vec3 position, std::string name, bool active = true);
	Cubic(glm::vec3 position, glm::vec3 direction, std::string name, bool active = true);
	virtual ~Cubic() {};
	virtual void initialize_object();

protected:
	virtual void update(float delta);

	std::vector<Vertex> set_cubic_vertices();
	std::vector<unsigned int> set_cubic_indices();
	std::vector<Texture> set_cubic_textures(int texture_num, std::string texture_path[], Texture_Type texture_type[]);
	unsigned int texture_from_file(std::string filename);

private:
	void initialize_cubic();
};