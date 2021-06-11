#pragma once

#include "TestObject/Cubic.h"

class PointLightObject : public Cubic
{
public:
	PointLightObject(std::string name, bool active = true);
	PointLightObject(glm::vec3 position, std::string name, bool active = true);
	PointLightObject(glm::vec3 position, glm::vec3 direction, std::string name, bool active = true);
	virtual ~PointLightObject() {};
	virtual void initialize_object();

private:
	void initialize_point_light( );
};