#include "Component/TransformComponent.h"
#include <iostream>

COMPONENT_DEFINITION(Component, TransformComponent);

const float TransformComponent::scale_min = 0.01;
const float TransformComponent::scale_max = 100.0;

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

TransformComponent::~TransformComponent()
{
	GameObject* parent_object = nullptr;
	std::shared_ptr<TransformComponent> parent_temp = parent_trans.lock();

	if (parent_temp != nullptr)
	{
		parent_object = parent_temp->gameobject;
	}

	for (auto it = chilldren_trans.begin(); it != chilldren_trans.end(); ++it)
	{
		std::shared_ptr<TransformComponent> child = it->second.lock();
		child->set_parent(parent_object);
	}

	if (parent_temp != nullptr)
	{
		parent_temp->chilldren_trans.erase(get_object_hash());
	}
}

void TransformComponent::initialize()
{
	//parent_trans = nullptr;
	position = glm::vec3(0.0);
	rotation = glm::quat(0.0, 0.0, 0.0, 1.0);
	scale = 1.0;

	global_mode = false;
	dirty_mark = false;

	g_position = glm::vec3(0.0);
	g_rotation = glm::quat(0.0, 0.0, 0.0, 1.0);
	g_scale = 1.0;
	trans_matrix = glm::mat4(1.0);
}

glm::mat4 TransformComponent::get_trans_matrix()
{
	return trans_matrix;
}

void TransformComponent::update(float delta)
{
	update_transform();
	cal_trans_matrix();
}

void TransformComponent::update_transform()
{
	// make sure that parent has been updated
	std::shared_ptr<TransformComponent> parent_temp = parent_trans.lock();
	if (parent_temp != nullptr)
	{
		parent_temp->update_transform();
	}

	// update current transform
	// global
	if (global_mode)
	{
		global_mode = false;
		dirty_mark = false;
		// update global state (nothing needs to do here, it has been finished in other method)
		// cal local state (need parent state)
		global_to_local();
		return;
	}
	// local
	else	if (dirty_mark)
	{
		dirty_mark = false;
		// update local state (nothing needs to do here, it has been finished in other method)
		// cal global state (need parent state)
		local_to_global();
		return;
	}
}

void TransformComponent::cal_trans_matrix()
{
	// use global state
	trans_matrix = glm::mat4(1.0);
	trans_matrix = glm::translate(trans_matrix, g_position);
	trans_matrix = trans_matrix * glm::mat4_cast(g_rotation);
	trans_matrix = glm::scale(trans_matrix, glm::vec3( clamp(g_scale, scale_min, scale_max) )); // range scale
}

void TransformComponent::rename(std::size_t name) // called by gameobject, don't call it yourself
{
	std::shared_ptr<TransformComponent> parent_temp = parent_trans.lock();
	if (parent_temp != nullptr)
	{
		std::size_t pre_name = get_object_hash();
		parent_temp->chilldren_trans.erase(pre_name);
		parent_temp->chilldren_trans.insert({ name, this->gameobject->transform });
	}
}

void TransformComponent::set_parent(GameObject* gameobject)
{
	// check whether the new parent is a child
	if (gameobject != nullptr)
	{
		std::weak_ptr<TransformComponent> check_trans = gameobject->transform;
		while (check_trans.lock() != nullptr)
		{
			if (check_trans.lock().get() == this)
				return;

			check_trans = check_trans.lock()->parent_trans;
		}
	}
	// update current transform to load previous changes
	update_transform();
	// remove it from previous parent
	std::shared_ptr<TransformComponent> parent_temp = parent_trans.lock();
	if (parent_temp != nullptr)
	{
		parent_temp->chilldren_trans.erase(get_object_hash());
	}
	parent_trans.reset();
	// set new parent and update parent's transform
	if (gameobject != nullptr)
	{
		gameobject->transform->update_transform();
		auto parents_children_list = &(gameobject->transform->chilldren_trans);
		// check name is not repeated
		int i = 0;
		std::string new_name;
		std::size_t object_hash = get_object_hash();
		while (parents_children_list->find(object_hash) != parents_children_list->end())
		{
			++i;
			new_name = "object" + std::to_string(i);
			object_hash = std::hash<std::string>() (new_name);
		}
		this->gameobject->rename(new_name);
		// insert into new parent
		parents_children_list->insert({ get_object_hash(),this->gameobject->transform });
		parent_trans = gameobject->transform;
	}
	// cal new local state
	global_to_local();
	dirty_mark = false;

	std::cout << "current: " << this << ", ";
	std::cout << "parent: " << parent_trans.lock().get() << std::endl;
}


void TransformComponent::local_to_global()
{
	std::shared_ptr<TransformComponent> parent_temp = parent_trans.lock();
	if (parent_temp != nullptr)
	{
		glm::vec3 p_position = parent_temp->g_position;
		glm::quat p_rotation = parent_temp->g_rotation;
		float p_scale = parent_temp->g_scale;

		g_position = p_position + p_scale * glm::mat3_cast(p_rotation) * position;
		g_rotation = p_rotation * rotation;
		g_scale = p_scale * scale;
	}
	else
	{
		g_position = position;
		g_rotation = rotation;
		g_scale = scale;
	}
}

void TransformComponent::global_to_local()
{
	std::shared_ptr<TransformComponent> parent_temp = parent_trans.lock();
	if (parent_temp != nullptr)
	{
		glm::vec3 p_position = parent_temp->g_position;
		glm::quat p_rotation = parent_temp->g_rotation;
		float p_scale = parent_temp->g_scale;

		position = glm::mat3_cast( glm::inverse(p_rotation) ) * (g_position - p_position) / p_scale;
		rotation = glm::inverse(p_rotation) * g_rotation;
		scale = g_scale / p_scale;
	}
	else
	{
		position = g_position;
		rotation = g_rotation;
		scale = g_scale;
	}
}

// setting method (local part)
void TransformComponent::set_local_position(const glm::vec3& local_position) 
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

	glm::quat local_rotation = glm::angleAxis(glm::radians(angle), glm::normalize(axis));

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

void TransformComponent::set_local_scale(const float local_scale)
{
	if (local_scale <= 0)
	{
		//warning: scale must be larger than 0
		return;
	}

	if (local_scale != scale)
	{
		dirty_mark = true;
		scale = local_scale;
	}
}

void TransformComponent::local_translate(const glm::vec3& trans)
{
	if (trans != glm::vec3(0.0f))
	{
		dirty_mark = true;
		position += trans;
	}
}

void TransformComponent::local_rotate(const glm::quat& rotate)
{
	if (rotate.w != 0)
	{
		dirty_mark = true;
		rotation = rotate * rotation;
	}
}

void TransformComponent::local_rotate(const glm::vec3& axis, const float angle)
{
	glm::quat rotate = glm::angleAxis(glm::radians(angle), glm::normalize(axis));

	if (rotate.w != 0)
	{
		dirty_mark = true;
		rotation = rotate * rotation;
	}
}

void TransformComponent::local_zoom(const float zoom)
{
	if (zoom <= 0.0)
	{
		//warning: scale must be larger than 0
		return;
	}

	if (zoom != 1.0)
	{
		dirty_mark = true;
		scale *= zoom;
	}
}

// ATTENTION: global mode will cancel all local manipulations
void TransformComponent::set_global_position_and_rotation(const glm::vec3& global_position, const glm::quat & global_rotation )
{
	global_mode = true;
	g_position = global_position;
	g_rotation = global_rotation;
}

void TransformComponent::set_global_position_and_rotation(const glm::vec3& global_position, const glm::vec3& axis, const float angle)
{
	if (axis == glm::vec3(0.0))
		return;

	glm::quat global_rotation = glm::angleAxis(glm::radians(angle), glm::normalize(axis));

	global_mode = true;
	g_position = global_position;
	g_rotation = global_rotation;
}

void TransformComponent::set_global_position_and_rotation(const glm::vec3 global_position, const float pitch, const float yaw, const float roll)
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

	glm::quat global_rotation = roll_rotation * pitch_rotation * yaw_rotation;

	global_mode = true;
	g_position = global_position;
	g_rotation = global_rotation;
}