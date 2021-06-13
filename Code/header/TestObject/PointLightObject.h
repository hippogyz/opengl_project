#pragma once

#include "TestObject/Cubic.h"

class PointLightObject : public Cubic
{
public:
	PointLightObject(std::string name, bool active = false);
	PointLightObject(glm::vec3 position, std::string name, bool active = false);
	virtual ~PointLightObject() {};
	virtual void initialize_object();

private:
	void initialize_point_light( );
};