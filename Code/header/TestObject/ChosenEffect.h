#pragma once

#include "GameObject.h"

// this object need a parent, and the parent should support stencil render with normal set
// for each object, there should be only one chosen effect as its child

class ChosenEffect : public GameObject
{
public:
	ChosenEffect(GameObject* parent, std::string name = "chosen effect", bool active = false);
	virtual ~ChosenEffect() {};
	virtual void initialize_object();

protected:
	virtual void update(float delta);

	void initialize_chosen_effect();
	GameObject * const parent;
};