#include "Component/TransformComponent.h"
#include <iostream>

COMPONENT_DEFINITION( Component, TransformComponent )

const float scale_min = 0.01;
const float scale_max = 100.0;

TransformComponent::TransformComponent(GameObject* gameobject, int order) : Component(gameobject, order)
{
	initialize();
}

glm::mat4 TransformComponent::get_trans_matrix()
{
	return trans_matrix;
}

void TransformComponent::update(float delta)
{

}

void TransformComponent::start()
{

}

void TransformComponent::initialize()
{
	position = glm::vec3(1.0);
	rotation = glm::quat(0.0, 0.0, 0.0, 1.0);
	scale = glm::vec3(1.0);

	global_mode = false;
	dirty_mark = false;
	trans_matrix = glm::mat4(1.0);
	local_trans_matrix = glm::mat4(1.0);
}

glm::mat4 TransformComponent::cal_trans_matrix()
{
	if (global_mode)
	{
		global_mode = false;
		dirty_mark = false;
		return trans_matrix;
	}

	if (dirty_mark)
	{
		//local_trans_matrix = XXXXX; scale - rotation - translate
		//////////////////////////////
	}

	if (parent_trans != nullptr)
	{
		trans_matrix = parent_trans->cal_trans_matrix() * local_trans_matrix;
	}

	dirty_mark = false;
	return trans_matrix;
}