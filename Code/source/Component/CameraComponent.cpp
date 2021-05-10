#include "Component/CameraComponent.h"
#include "Game.h"
#include "GameObject.h"
#include "Component/TransformComponent.h"
#include "RenderManager.h"
#include <memory>

COMPONENT_DEFINITION(Component, CameraComponent);

CameraComponent::CameraComponent(GameObject* gameobject, int order) : Component(gameobject, order)
{

}

CameraComponent::~CameraComponent()
{

}

void CameraComponent::start()
{
	// register camera to render manager
	std::weak_ptr<CameraComponent> itself = getComponent<CameraComponent>();	// rewrite this if there is child class of camera
	Game::access().render_manager->assign_camera(itself);
}

glm::vec3 CameraComponent::get_position()
{
	return gameobject->transform->g_position;
}

glm::mat4 CameraComponent::get_view()
{
	glm::vec3 position = gameobject->transform->g_position;
	glm::vec3 up = glm::vec3(0.0, 1.0, 0.0);
	glm::vec3 front = glm::mat3_cast(gameobject->transform->g_rotation) * glm::vec3(1.0, 0.0, 0.0);

	return glm::lookAt(position, position + front, up);
}