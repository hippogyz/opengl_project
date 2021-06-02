#include "Light.h"
#include <string>

Light::Light(bool is_active, glm::vec3 diffuse, glm::vec3 specular):
	is_active(is_active), diffuse(diffuse), specular(specular)
{

}

const char* Light::dir_name = "dir_light";
const char* Light::point_name = "point_light";
const char* Light::spot_name = "spot_light";

void Light::setLightWithName(Shader& shader_prog)
{
	setLight(shader_prog, light_type);
}

void Light::setLightWithIndex(Shader& shader_prog, int index)
{
	std::string name = std::string(light_type) + '[' + char(index + int('0')) + ']';
	setLight(shader_prog, name.c_str());
}

DirLight::DirLight(bool is_active, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction):
	Light(is_active, diffuse, specular), direction(direction)
{
	light_type = Light::dir_name;
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
	light_type = Light::point_name;
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
	light_type = Light::spot_name;
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