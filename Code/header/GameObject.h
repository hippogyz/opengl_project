#pragma once

#include <vector>
#include <memory>

class Component;

class GameObject {
public: // member
	std::vector< std::unique_ptr<Component> > component_list;
	bool is_active;

public: // method
	void update(float delta);
	
	template < typename ComponentName > 
		ComponentName& GetComponent();
	template < typename ComponentName, typename... Args >
		void AddComponent(Args&&... args);
	template < typename ComponentName >
		bool RemoveComponent();
};