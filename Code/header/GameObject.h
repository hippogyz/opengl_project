#pragma once

#include <vector>
#include <memory>

class Component;

class GameObject {
public: // member
	std::vector< std::unique_ptr<Component> > component_list;
	bool is_active;

public: // method
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
	std::vector< unsigned int > removing_component;
	std::vector< std::unique_ptr<Component> > component_buffer;
	
private: // method
	void arrangeComponent();
};