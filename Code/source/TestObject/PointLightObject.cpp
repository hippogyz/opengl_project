#include "TestObject/PointLightObject.h"

#include <iostream>
#include "Component/TransformComponent.h"
#include "Component/RenderComponent.h"
#include "Component/LightComponent.h"


PointLightObject::PointLightObject(std::string name, bool active) : Cubic(name, active)
{

}

PointLightObject::PointLightObject(glm::vec3 position, std::string name, bool active) : Cubic(position, name, active)
{
	transform->set_local_scale(0.5f);
}

PointLightObject::PointLightObject(glm::vec3 position, glm::vec3 direction, std::string name, bool active) : Cubic(position, direction, name, active)
{
	transform->set_local_scale(0.5f);
}

void PointLightObject::initialize_object()
{
	initialize_point_light();
}

void PointLightObject::initialize_point_light()
{
	static std::vector<Vertex> vertices = set_cubic_vertices();

	static std::vector<unsigned int> indices = set_cubic_indices();

	static std::vector<Texture> textures = set_cubic_textures(0, NULL, NULL);

	static const char* vs_path = "opengl_project/Code/shader/vertex_shader.vs";
	static const char* fs_path = "opengl_project/Code/shader/ref_frag_shader.fs";

	renderer->initialize_renderer("light_source", vertices, indices, textures, vs_path, fs_path);
	std::cout << "point light source builded " << std::endl;

	auto light = addComponent<LightComponent>(this);

	light->initializeLight<PointLight>(true, glm::vec3(0.7), glm::vec3(1.0), glm::vec3(0.0));
	light->update_trans_information();
}
