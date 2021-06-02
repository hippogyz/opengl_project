#pragma once
#include <memory>
#include <vector>

class LightComponent;
class Shader;

class LightManager
{
	friend class RenderManager;

public:
	LightManager();
	~LightManager();
	void assign_light(std::weak_ptr<LightComponent> light_c);
	void set_light(Shader& shader_prog);

private:
	std::vector< std::weak_ptr< LightComponent > > point_light;
	std::vector< std::weak_ptr< LightComponent > > dir_light;
	std::vector< std::weak_ptr< LightComponent > > spot_light;

	const int point_limit;
	const int dir_limit;
	const int spot_limit;
};