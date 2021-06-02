#include "Light.h"
#include <string>

Light::Light(bool is_active, glm::vec3 diffuse, glm::vec3 specular):
	is_active(is_active), diffuse(diffuse), specular(specular)
{

}

DirLight::DirLight(bool is_active, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction):
	Light(is_active, diffuse, specular), direction(direction)
{
	light_type = "dir_light";
}

void DirLight::setLight(Shader& shader_prog, const char* name)
{
	shader_prog.useShader();
	shader_prog.setBool((name + std::string(".is_active")).c_str(), is_active);
	shader_prog.setVec3((name + std::string(".direction")).c_str(), direction);
	shader_prog.setVec3((name + std::string(".diffuse")).c_str(), diffuse);
	shader_prog.setVec3((name + std::string(".specular")).c_str(), specular);
}

PointLight::PointLight(bool is_active, glm::vec3 diffuse, glm::vec3 specular,
	glm::vec3 position, float decay_0, float decay_1, float decay_2) :
	Light(is_active, diffuse, specular), position(position), decay_0(decay_0), decay_1(decay_1), decay_2(decay_2)
{
	light_type = "point_light";
}

void PointLight::setLight(Shader& shader_prog, const char* name)
{
	shader_prog.useShader();
	shader_prog.setBool((name + std::string(".is_active")).c_str(), is_active);
	shader_prog.setVec3((name + std::string(".position")).c_str(), position);
	shader_prog.setVec3((name + std::string(".diffuse")).c_str(), diffuse);
	shader_prog.setVec3((name + std::string(".specular")).c_str(), specular);
	shader_prog.setFloat((name + std::string(".decay_0")).c_str(), decay_0);
	shader_prog.setFloat((name + std::string(".decay_1")).c_str(), decay_1);
	shader_prog.setFloat((name + std::string(".decay_2")).c_str(), decay_2);
}

SpotLight::SpotLight(bool is_active, glm::vec3 diffuse, glm::vec3 specular,
	glm::vec3 position, glm::vec3 direction,
	float decay_0, float decay_1, float decay_2, float inner_cone, float outer_cone) :
	PointLight(is_active, diffuse, specular, position, decay_0, decay_1, decay_2),
	direction(direction), inner_cone(inner_cone), outer_cone(outer_cone)
{
	light_type = "spot_light";
}

void SpotLight::setLight(Shader& shader_prog, const char* name)
{
	shader_prog.useShader();
	shader_prog.setBool((name + std::string(".is_active")).c_str(), is_active);
	shader_prog.setVec3((name + std::string(".position")).c_str(), position);
	shader_prog.setVec3((name + std::string(".direction")).c_str(), direction);
	shader_prog.setVec3((name + std::string(".diffuse")).c_str(), diffuse);
	shader_prog.setVec3((name + std::string(".specular")).c_str(), specular);
	shader_prog.setFloat((name + std::string(".decay_0")).c_str(), decay_0);
	shader_prog.setFloat((name + std::string(".decay_1")).c_str(), decay_1);
	shader_prog.setFloat((name + std::string(".decay_2")).c_str(), decay_2);
	shader_prog.setFloat((name + std::string(".inner_cone")).c_str(), inner_cone);
	shader_prog.setFloat((name + std::string(".outer_cone")).c_str(), outer_cone);
}