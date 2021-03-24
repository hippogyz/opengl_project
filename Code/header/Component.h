#pragma once

#include "GameObject.h"

class Component {
public: // member
	static const std::size_t Type;
	int order;
	bool is_active;

public: // method
	void uniform_update(float delta) {};
	virtual void update(float delta) {};
	virtual bool isComponentType(const std::size_t type) { return true; };
};
