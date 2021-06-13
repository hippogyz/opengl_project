#pragma once

#include "TestObject/Cubic.h"

class SpotLightObject : public Cubic
{
public:
	SpotLightObject(std::string name, bool active = false);
	SpotLightObject(glm::vec3 position, std::string name, bool active = false);
	SpotLightObject(glm::vec3 position, glm::vec3 direction, std::string name, bool active = false);
	virtual ~SpotLightObject() {};
	virtual void initialize_object();

private:
	void initialize_spot_light();
};