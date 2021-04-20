#include "Component.h"

// basic definition of Component class
const std::size_t Component::Type = std::hash<std::string>() (COMPONENT_TO_STRING(Component));
bool Component::isComponentType(const std::size_t type) const		{	return type == Type;	}
std::size_t Component::getComponentType() const		{	return Type;	}


Component::Component(GameObject* gameobject, int order) : gameobject(gameobject), order(order)
{
	is_active = true;
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

void Component::rename(std::size_t name)
{

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

bool Component::is_removed()
{
	return removed;
}

std::size_t Component::get_object_hash()
{
	return gameobject->object_hash;
}