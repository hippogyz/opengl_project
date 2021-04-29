#pragma once

#include "GameObject.h"

class Cubic : public GameObject
{
public:
	Cubic(std::string name, bool active = true);

private:
	void initialize_cubic();
};