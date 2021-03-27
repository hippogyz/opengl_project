#pragma once

#include "Component.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/quaternion.hpp>

class TransformComponent : public Component {

	COMPONENT_DECLARATION(TransformComponent)

public:
	TransformComponent* parent_trans;
	// local
	glm::vec3 position;
	glm::quat rotation;
	glm::vec3 scale;

public:
	TransformComponent(GameObject* gameobject, int order = TRANSFORM_ORDER);
	virtual ~TransformComponent() {};

	glm::mat4 get_trans_matrix();
	/*
	void set_parent(GameObject* parent_object);
	
	void set_local_rotation(const glm::quat& local_rotation); 
	void set_local_rotation(const glm::vec3& axis, const float angle);
	void set_local_rotation(const float pitch, const float yaw, const float roll);
	void local_rotate(...);

	void global_translate(...);
	void global_rotate(...);
	void global_scale(...);

	glm::vec3 get_global_position();
	*/

protected:
	virtual void update(float delta);
	virtual void start();

private:
	bool global_mode;
	bool dirty_mark;
	glm::mat4 trans_matrix; 
	glm::mat4 local_trans_matrix;

	static const float scale_min;
	static const float scale_max;

private:
	void initialize();
	glm::mat4 cal_trans_matrix();
};