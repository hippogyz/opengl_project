#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <memory>
#include <string>

class Shader;
class Model;
class Camera;
class Light;
class GameObject;
struct Vertex;
struct Texture;

class RenderManager {

	friend class Game;

public:
	std::vector< std::shared_ptr<Shader> > shaders;
	std::vector< std::shared_ptr<Model> > models;
	std::weak_ptr<Camera> camera; // camera component
	// light.....

public:
	RenderManager();
	~RenderManager();

	void BeforeRender(float delta); // update camera for shaders // only called by game, it would be better set as private?
	void AfterRender(float delta);

	std::weak_ptr<Model> assign_model(std::string model_name, std::vector< Vertex > vertices, 
																	std::vector< unsigned int > indices,	std::vector<Texture>textures );
	std::weak_ptr<Model> assign_model( std::string model_path );
	std::weak_ptr<Shader> assign_shader_VF(const char* vs_path, const char* fs_path );

private:
	GLFWwindow* window;

	float window_time;
	double mouse_position[2];
	int window_size[2];

	glm::mat4 projection;

	static const unsigned int SCR_WIDTH;
	static const unsigned int SCR_HEIGHT;

private:
	void initializeOpenGL();

	void clearBuffer(GLenum config);
	static void framebuffer_size_callback(GLFWwindow* window, int width, int height);// link to input_controller
	static void cursor_move_callback(GLFWwindow* window, double cursor_x, double cursor_y); // link to input_controller
	void process_input(GLFWwindow* window);// link to input_controller
};