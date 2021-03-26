#pragma once

#include "Component.h"

class TransformComponent : public Component {
public: // type
	static const std::size_t Type;
	virtual bool isComponentType(const std::size_t type) const;

public:
	TransformComponent(bool is_active, int order);

};