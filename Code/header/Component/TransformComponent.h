#pragma once

#include "Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>
#include <vector>
#include <unordered_map>

// right hand, y-axis denotes up-direction, x-axis denotes original right-direction

class TransformComponent : public Component {

	COMPONENT_DECLARATION(TransformComponent)

public:
	std::weak_ptr<TransformComponent> parent_trans;
	std::unordered_map < std::size_t, std::weak_ptr<TransformComponent>  > chilldren_trans;

	// local
	glm::vec3 position;
	glm::quat rotation;
	float scale; // only support uniform scaling

public:
	TransformComponent(GameObject* gameobject, int order = TRANSFORM_ORDER);
	// TransformComponent(GameObject* gameobject, glm::vec3 position, int order = TRANSFORM_ORDER);
	// TransformComponent(GameObject* gameobject, glm::vec3 position, glm::quat rotation, int order = TRANSFORM_ORDER);
	// TransformComponent(GameObject* gameobject, glm::vec3 position, float scale, int order = TRANSFORM_ORDER);
	// TransformComponent(GameObject* gameobject, glm::vec3 position, glm::quat rotation, float scale, int order = TRANSFORM_ORDER);
	// TransformComponent(GameObject* gameobject, float scale, int order = TRANSFORM_ORDER);
	virtual ~TransformComponent();

	glm::mat4 get_trans_matrix();

	virtual void rename(std::size_t name);
	void set_parent(GameObject* parent_object);
	
	void set_local_position(const glm::vec3& local_position);
	void set_local_rotation(const glm::quat& local_rotation);
	void set_local_rotation(const glm::vec3& axis, const float angle);
	void set_local_rotation(const float pitch, const float yaw, const float roll);
	void set_local_scale(const float local_scale);

	void local_translate(const glm::vec3& trans);
	void local_rotate(const glm::quat & rotate);
	void local_rotate(const glm::vec3& axis, const float angle);
	void local_zoom(const float zoom);

	void set_global_position_and_rotation(const glm::vec3& global_position, const glm::quat& global_rotation);
	void set_global_position_and_rotation(const glm::vec3& global_position, const glm::vec3& axis, const float angle);
	void set_global_position_and_rotation(const glm::vec3 global_position, const float pitch, const float yaw, const float roll);

protected:
	virtual void update(float delta);

private:
	bool global_mode;
	bool dirty_mark;

	glm::vec3 g_position;
	glm::quat g_rotation;
	float g_scale;
	glm::mat4 trans_matrix;

private:
	void initialize();

	void update_transform();
	void cal_trans_matrix();

	void local_to_global();
	void global_to_local();

private:
	static const float scale_min;
	static const float scale_max;
};