#pragma once

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <memory>

class Shader;
class Model;
class Camera;


class RenderManager {
public:
	std::vector< std::shared_ptr<Shader> > shaders;
	std::vector< std::shared_ptr<Model> > models;
	Camera* camera; // camera component -------------------------------------should use weak ptr

public:
	RenderManager();
	~RenderManager();

	void BeforeRender(); // update camera for shaders
	void AfterRender(); 

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
	void processInput(GLFWwindow* window);// link to input_controller
};