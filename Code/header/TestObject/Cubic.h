#pragma once

#include "GameObject.h"

class Cubic : public GameObject
{
public:
	Cubic(std::string name, bool active = true);
	virtual ~Cubic() {};

protected:
	virtual void update(float delta);

private:
	void initialize_cubic();
};