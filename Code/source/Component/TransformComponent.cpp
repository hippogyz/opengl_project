#include "Component/TransformComponent.h"
#include <iostream>

COMPONENT_DEFINITION( Component, TransformComponent )

TransformComponent::TransformComponent(bool is_active, int order) : Component(is_active, order)
{

}