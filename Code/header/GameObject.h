#pragma once

#include <vector>
#include <memory>

class Component;

class GameObject {
public: // member
	std::vector< std::unique_ptr<Component> > component_list;
	bool is_active;

public: // method
	GameObject(bool active);
	virtual ~GameObject();

	void uniform_update(float delta);
	virtual void start();
	virtual void update(float delta);
	
	template < typename ComponentName > 
		ComponentName& getComponent();
	template < typename ComponentName, typename... Args >
		void addComponent(Args&&... args);
	template < typename ComponentName >
		void removeComponent();

private: // member
	bool first_update;
	std::vector< unsigned int > remove_buffer;
	std::vector< std::unique_ptr<Component> > add_buffer;
	
private: // method
	void arrangeComponent();
};