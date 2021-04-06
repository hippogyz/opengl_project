#include "Component/TransformComponent.h"
#include <iostream>

COMPONENT_DEFINITION( Component, TransformComponent )

const float scale_min = 0.01;
const float scale_max = 100.0;

template <typename T>
static const T clamp(T target, T min, T max)
{
	target = (target < max) ? target : max;
	target = (target > min) ? target : min;
	return target;
}

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
	parent_trans = nullptr;
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

	// global_mode == false   now
	// update local trans matrix if dirty mark is true
	if (dirty_mark)
	{
		//local_trans_matrix = XXXXX; scale - rotation - translate
		//////////////////////////////
	}

	// check parent trans matrix
	if (parent_trans != nullptr)
	{
		trans_matrix = parent_trans->cal_trans_matrix() * local_trans_matrix;
	}

	dirty_mark = false;
	return trans_matrix;
}

void TransformComponent::set_parent(GameObject* gameobject)
{
	parent_trans = gameobject->transform.get();
}

// setting method (local part)
void TransformComponent::set_local_position(const glm::vec3 local_position) 
{
	if (local_position != position)
	{
		dirty_mark = true;
		position = local_position;
	}
}

void TransformComponent::set_local_rotation(const glm::quat& local_rotation)
{
	if (local_rotation != rotation)
	{
		dirty_mark = true;
		rotation = local_rotation;
	}
}

void TransformComponent::set_local_rotation(const glm::vec3& axis, const float angle) 
{
	if (axis == glm::vec3(0.0))
		return;

	const glm::quat local_rotation = glm::angleAxis(glm::radians(angle), glm::normalize(axis));

	if (local_rotation != rotation)
	{
		dirty_mark = true;
		rotation = local_rotation;
	}
}
void TransformComponent::set_local_rotation(const float pitch, const float yaw, const float roll)
{
	float ry = glm::radians(yaw);
	float rp = glm::radians(clamp(pitch, -90.0f, 90.0f));
	float rr = glm::radians(roll);

	// y -> yaw -> z -> pitch -> x -> roll
	glm::vec3 y_axis = glm::vec3(0.0, 1.0, 0.0);
	glm::quat yaw_rotation = glm::angleAxis(ry, glm::normalize(y_axis));
	glm::vec3 z_axis = glm::vec3(sin(ry), 0.0, cos(ry));
	glm::quat pitch_rotation = glm::angleAxis(rp, glm::normalize(z_axis));
	glm::vec3 x_axis = glm::vec3(cos(ry) * cos(rp), sin(rp), -sin(ry) * cos(rp));
	glm::quat roll_rotation = glm::angleAxis(rr, glm::normalize(x_axis));

	glm::quat local_rotation = roll_rotation * pitch_rotation * yaw_rotation;
	if (local_rotation != rotation)
	{
		dirty_mark = true;
		rotation = local_rotation;
	}
}

void TransformComponent::set_local_scale(const glm::vec3 local_scale)
{
	if (local_scale != scale)
	{
		dirty_mark = true;
		scale = local_scale;
	}
}