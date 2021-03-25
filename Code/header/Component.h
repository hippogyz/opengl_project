#pragma once

#include <memory>

class Component {
public: // member
	static const std::size_t Type;
	int order;
	bool is_active;

public: // method
	Component(bool is_active, int order);
	void uniform_update(float delta) {};
	virtual void update(float delta) {};
	virtual bool isComponentType(const std::size_t type) { return true; };

	bool remove_call();

private:
	bool removed;
};
