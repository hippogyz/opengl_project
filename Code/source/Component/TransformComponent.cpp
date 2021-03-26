#include "Component/TransformComponent.h"

const std::size_t TransformComponent::Type = std::hash<std::string>() (std::string("TransformComponent"));

bool TransformComponent::isComponentType(std::size_t type) const 
{
	if (type == Type)
		return true;
	return Component::isComponentType(type);
}


TransformComponent::TransformComponent(bool is_active, int order) : Component(is_active, order)
{

}