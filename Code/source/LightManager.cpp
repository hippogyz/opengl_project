#include "LightManager.h"

#include "Light.h"
#include "Component/LightComponent.h"

LightManager::LightManager() : point_limit(4) , dir_limit(1), spot_limit(1)
{

}

LightManager::~LightManager()
{

}

void LightManager::assign_light(std::weak_ptr<LightComponent> light_c)
{
	const char* type = light_c.lock()->light->light_type;
	
	if (type == Light::point_name && point_light.size() < point_limit)
	{
		point_light.push_back(light_c);
	}
	else if (type == Light::dir_name && dir_light.size() < dir_limit)
	{
		dir_light.push_back(light_c);
	}
	else if (type == Light::spot_name && spot_light.size() < spot_limit)
	{
		spot_light.push_back(light_c);
	}
}

void LightManager::set_light(Shader& shader_prog)
{
	int list_size;

	// point part
	list_size = point_light.size();

	for (int i = 0; i < list_size; ++i)
	{
		if (point_limit > 1)
		{
			point_light[i].lock()->light->setLightWithIndex(shader_prog, i);
		}
		else
		{
			point_light[i].lock()->light->setLightWithName(shader_prog);
		}
	}

	point_light.clear();

	// dir part
	list_size = dir_light.size();

	for (int i = 0; i < list_size; ++i)
	{
		if (dir_limit > 1)
		{
			dir_light[i].lock()->light->setLightWithIndex(shader_prog, i);
		}
		else
		{
			dir_light[i].lock()->light->setLightWithName(shader_prog);
		}
	}

	dir_light.clear();

	// spot part
	list_size = spot_light.size();

	for (int i = 0; i < list_size; ++i)
	{
		if (spot_limit > 1)
		{
			spot_light[i].lock()->light->setLightWithIndex(shader_prog, i);
		}
		else
		{
			spot_light[i].lock()->light->setLightWithName(shader_prog);
		}
	}

	spot_light.clear();
}