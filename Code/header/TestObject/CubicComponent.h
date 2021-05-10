#pragma once

#include "Component.h"

class CubicComponent : public Component
{
	COMPONENT_DECLARATION(CubicComponent)

public:
	CubicComponent(GameObject* gameobject, int order = 1);
	virtual ~CubicComponent() {};

protected:
	virtual void update(float delta);
};