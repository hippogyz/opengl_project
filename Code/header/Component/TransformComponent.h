#pragma once

#include "Component.h"

class TransformComponent : public Component {

	COMPONENT_DECLARATION(TransformComponent)

public:
	TransformComponent(bool is_active, int order);
};