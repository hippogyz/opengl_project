#pragma once

#include "Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

// right hand, y-axis denotes up-direction, x-axis denotes original right-direction

class TransformComponent : public Component {

	COMPONENT_DECLARATION(TransformComponent)

public:
	TransformComponent* parent_trans;
	// local
	glm::vec3 position;
	glm::quat rotation;
	float scale; // only support uniform scaling

public:
	TransformComponent(GameObject* gameobject, int order = TRANSFORM_ORDER);
	virtual ~TransformComponent() {};

	glm::mat4 get_trans_matrix();

	void set_parent(GameObject* parent_object);
	
	void set_local_position(const glm::vec3& local_position);
	void set_local_rotation(const glm::quat& local_rotation);
	void set_local_rotation(const glm::vec3& axis, const float angle);
	void set_local_rotation(const float pitch, const float yaw, const float roll);
	void set_local_scale(const float local_scale);
	//void local_translate(...);
	//void local_rotate(...);
	//void local_zoom(...);

	//void set_global_position_and_rotation(...);

protected:
	virtual void update(float delta);
	virtual void start();

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