#pragma once
#include "Component.h"

#include "Light.h"
#include <memory>

class LightComponent: public Component
{
	COMPONENT_DECLARATION(LightComponent);

public:
	LightComponent(GameObject* gameobject, int order = 15);
	
	template <typename LightType, typename... Args>
	void initializeLight(Args&&... args);

	std::shared_ptr<Light> light;
	void update_trans_information();

protected:
	virtual void update(float delta);

};

template <typename LightType, typename... Args>
void LightComponent::initializeLight(Args&&... args)
{
	light = std::make_shared<LightType>(std::forward<Args>(args)...);
	this->is_active = light->is_active;
}