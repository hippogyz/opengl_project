#include "Light.h"
#include <string>

Light::Light(bool is_active, glm::vec3 diffuse, glm::vec3 specular):
	is_active(is_active), diffuse(diffuse), specular(specular)
{

}

DirLight::DirLight(bool is_active, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction):
	Light(is_active, diffuse, specular), direction(direction)
{

}

void DirLight::setLight(Shader& shader_prog)
{
	shader_prog.useShader();
	shader_prog.setBool("dir_light.is_active", is_active);
	shader_prog.setVec3("dir_light.direction", direction);
	shader_prog.setVec3("dir_light.diffuse", diffuse);
	shader_prog.setVec3("dir_light.specular", specular);
}

void DirLight::setLight(Shader& shader_prog, int order)
{
	shader_prog.useShader();
	std::string mem_name = std::string("dir_light[") + char(order + int('0')) + std::string("].");

	shader_prog.setBool( (mem_name + "is_active").c_str(), is_active);
	shader_prog.setVec3((mem_name + "direction").c_str(), direction);
	shader_prog.setVec3((mem_name + "diffuse").c_str(), diffuse);
	shader_prog.setVec3((mem_name + "specular").c_str(), specular);
}

PointLight::PointLight(bool is_active, glm::vec3 diffuse, glm::vec3 specular,
	glm::vec3 position, float decay_0, float decay_1, float decay_2) :
	Light(is_active, diffuse, specular), position(position), decay_0(decay_0), decay_1(decay_1), decay_2(decay_2)
{

}

void PointLight::setLight(Shader& shader_prog)
{
	shader_prog.useShader();
	shader_prog.setBool("point_light.is_active", is_active);
	shader_prog.setVec3("point_light.position", position);
	shader_prog.setVec3("point_light.diffuse", diffuse);
	shader_prog.setVec3("point_light.specular", specular);
	shader_prog.setFloat("point_light.decay_0", decay_0);
	shader_prog.setFloat("point_light.decay_1", decay_1);
	shader_prog.setFloat("point_light.decay_2", decay_2);
}

void PointLight::setLight(Shader& shader_prog, int order)
{
	shader_prog.useShader();
	std::string mem_name = std::string("point_light[") + char(order + int('0')) + std::string("].");

	shader_prog.setBool((mem_name + "is_active").c_str(), is_active);
	shader_prog.setVec3((mem_name + "position").c_str(), position);
	shader_prog.setVec3((mem_name + "diffuse").c_str(), diffuse);
	shader_prog.setVec3((mem_name + "specular").c_str(), specular);
	shader_prog.setFloat((mem_name + "decay_0").c_str(), decay_0);
	shader_prog.setFloat((mem_name + "decay_1").c_str(), decay_1);
	shader_prog.setFloat((mem_name + "decay_2").c_str(), decay_2);
}

SpotLight::SpotLight(bool is_active, glm::vec3 diffuse, glm::vec3 specular,
	glm::vec3 position, glm::vec3 direction,
	float decay_0, float decay_1, float decay_2, float inner_cone, float outer_cone) :
	PointLight(is_active, diffuse, specular, position, decay_0, decay_1, decay_2),
	direction(direction), inner_cone(inner_cone), outer_cone(outer_cone)
{

}

void SpotLight::setLight(Shader& shader_prog)
{
	shader_prog.useShader();
	shader_prog.setBool("spot_light.is_active", is_active);
	shader_prog.setVec3("spot_light.position", position);
	shader_prog.setVec3("spot_light.direction", direction);
	shader_prog.setVec3("spot_light.diffuse", diffuse);
	shader_prog.setVec3("spot_light.specular", specular);
	shader_prog.setFloat("spot_light.decay_0", decay_0);
	shader_prog.setFloat("spot_light.decay_1", decay_1);
	shader_prog.setFloat("spot_light.decay_2", decay_2);
	shader_prog.setFloat("spot_light.inner_cone", inner_cone);
	shader_prog.setFloat("spot_light.outer_cone", outer_cone);
}

void SpotLight::setLight(Shader& shader_prog, int order) 
{
	shader_prog.useShader();
	std::string mem_name = std::string("spot_light[") + char(order + int('0')) + std::string("].");

	shader_prog.setBool((mem_name + "is_active").c_str(), is_active);
	shader_prog.setVec3((mem_name + "position").c_str(), position);
	shader_prog.setVec3((mem_name + "direction").c_str(), direction);
	shader_prog.setVec3((mem_name + "diffuse").c_str(), diffuse);
	shader_prog.setVec3((mem_name + "specular").c_str(), specular);
	shader_prog.setFloat((mem_name + "decay_0").c_str(), decay_0);
	shader_prog.setFloat((mem_name + "decay_1").c_str(), decay_1);
	shader_prog.setFloat((mem_name + "decay_2").c_str(), decay_2);
	shader_prog.setFloat((mem_name + "inner_cone").c_str(), inner_cone);
	shader_prog.setFloat((mem_name + "outer_cone").c_str(), outer_cone);
}