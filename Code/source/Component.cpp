#include "Component.h"

const std::size_t Component::Type = std::hash<std::string>() (std::string("Component"));

bool Component::isComponentType(const std::size_t type) const
{
	return type == Type;
}


Component::Component(bool is_active, int order) : is_active(is_active), order(order)
{
	first_update = true;
	removed = false;
}

void Component::uniform_update(float delta)
{
	if (!is_active) return;

	if (first_update)
	{
		start();
		first_update = false;
	}

	update(delta);
}

void Component::start()
{

}

void Component::update(float delta)
{

}


bool Component::remove_call()
{
	if (!removed)
	{
		removed = true;
		return true;
	}

	return false;
}