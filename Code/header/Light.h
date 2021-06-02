#pragma once
#include <glad/glad.h>

#include <iostream>
#include <stdio.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Shader.h"

class Light
{
public:
	Light(bool is_active, glm::vec3 diffuse, glm::vec3 specular);
	virtual void setLight(Shader& shader_prog, const char* name) = 0;
	void setLightWithName(Shader& shader_prog);
	void setLightWithIndex(Shader& shader_prog, int index);
	virtual ~Light() {};

	bool is_active;
	const char* light_type;

	static const char* dir_name;
	static const char* point_name;
	static const char* spot_name;

protected:
	glm::vec3 diffuse;
	glm::vec3 specular;
};

class DirLight : public Light {
public:
	DirLight(bool is_active, glm::vec3 diffuse, glm::vec3 specular, glm::vec3 direction);

	virtual void setLight(Shader& shader_prog, const char* name);
	virtual ~DirLight() {};

protected:
	glm::vec3 direction;
};

class PointLight : public Light {
public:
	PointLight(bool is_active, glm::vec3 diffuse, glm::vec3 specular, 
		glm::vec3 position, float decay_0 = 1.0, float decay_1 = 0.05, float decay_2 = 0.025);

	virtual void setLight(Shader& shader_prog, const char* name);
	virtual ~PointLight() {};

protected:
	glm::vec3 position;

	float decay_0;
	float decay_1;
	float decay_2;
};

class SpotLight : public PointLight {
public:
	SpotLight(bool is_active, glm::vec3 diffuse, glm::vec3 specular, 
		glm::vec3 position, glm::vec3 direction,
		float decay_0 = 1.0, float decay_1 = 0.05, float decay_2 = 0.025, float inner_cone = 0.95, float outer_cone = 0.85);

	virtual void setLight(Shader& shader_prog, const char* name);
	virtual ~SpotLight() {};

protected:
	glm::vec3 direction;

	float inner_cone;
	float outer_cone;
};