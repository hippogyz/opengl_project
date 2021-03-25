#include "Component.h"

const std::size_t Component::Type = 1;

Component::Component(bool is_active, int order) : is_active(is_active), order(order)
{
	removed = false;
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