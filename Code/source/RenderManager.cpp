#include "RenderManager.h"

#include <iostream>
#include "Game.h"
#include "Model.h"
#include "Shader.h"

const unsigned int RenderManager::SCR_WIDTH = 600;
const unsigned int RenderManager::SCR_HEIGHT = 600;

RenderManager::RenderManager()
{
    std::cout << "render manager building" << std::endl;

    window_time = 0;
    mouse_position[0] = 0.0;
    mouse_position[1] = 0.0;
    window_size[0] = SCR_WIDTH;
    window_size[1] = SCR_HEIGHT;

	initializeOpenGL();
}

RenderManager::~RenderManager()
{
    glfwTerminate();
}

void RenderManager::initializeOpenGL()
{    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MyOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
    }

    glfwMakeContextCurrent(window);
    //glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    //glfwSetCursorPosCallback(window, cursor_move_callback);
    //glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // fix cursor for First Personal Sight

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
    }

    glViewport(0, 0, SCR_WIDTH, SCR_HEIGHT);

    // enable depth test
    // ------------------
    glEnable(GL_DEPTH_TEST);
}

void RenderManager::BeforeRender(float delta)
{
    clearBuffer(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // update window
    window_time += delta;
    // update camera
    glm::vec3 camera_position = glm::vec3(0.0, 0.0, 3.0); // ---------------------
    glm::mat4 view = glm::lookAt(camera_position, camera_position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0)); // ---------------------- camera
    // update projection
    projection = glm::perspective(glm::radians(45.0f), float(window_size[0]) / float(window_size[1]), 0.1f, 100.0f);
    // update light

    for (auto&& shader : shaders)
    {
        shader->useShader();

        shader->setTrans("view", view);
        shader->setTrans("projection", projection);

        // fragment uniform
        shader->setFloat("u_time", window_time);
        shader->setFloat("u_mouse", float(mouse_position[0]), float(window_size[1]) - float(mouse_position[1]));
        shader->setFloat("u_resolution", float(window_size[0]), float(window_size[1]));

        shader->setFloat("camera_position", camera_position.x, camera_position.y, camera_position.z);
    }
}

void RenderManager::AfterRender(float delta)
{
    glfwSwapBuffers(window);
    glfwPollEvents();
}

std::weak_ptr<Model> RenderManager::assign_model(std::string model_name, std::vector< Vertex > vertices,
    std::vector< unsigned int > indices, std::vector<Texture>textures)
{
    std::size_t m_hash = std::hash<std::string>()(model_name);

    for (auto&& model : models)
    {
        if (model->model_hash == m_hash)
        {
            return std::weak_ptr<Model>(model);
        }
    }

    std::shared_ptr<Model> model = std::make_shared<Model>(model_name, vertices, indices, textures);
    models.push_back(model);
    return std::weak_ptr<Model>(model);
}

std::weak_ptr<Model> RenderManager::assign_model(std::string model_path)
{
    std::size_t m_hash = std::hash<std::string>() (model_path);

    for (auto&& model : models)
    {
        if (model->model_hash == m_hash)
        {
            return std::weak_ptr<Model> (model);
        }
    }

    std::shared_ptr<Model> model = std::make_shared<Model>(model_path);
    models.push_back(model);
    return std::weak_ptr<Model>(model);
}

std::weak_ptr<Shader> RenderManager::assign_shader_VF(const char* vs_path, const char* fs_path)
{
    std::size_t m_hash = std::hash<std::string>() (std::string(vs_path) + std::string(fs_path));

    for (auto&& shader : shaders)
    {
        if (shader->shader_hash == m_hash)
            return std::weak_ptr<Shader>(shader);
    }

    std::shared_ptr<Shader> shader = std::make_shared<Shader>();
    shader->initializeShaderVF(vs_path, fs_path);
    shaders.push_back(shader);
    return std::weak_ptr<Shader>(shader);
}


void RenderManager::clearBuffer(GLenum config)
{
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(config);
}

void RenderManager::framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void RenderManager::cursor_move_callback(GLFWwindow* window, double cursor_x, double cursor_y)
{
 //   Game::access().input_controller.XXXXX
/*
    if (first_cursor)
    {
        cursor_last_x = cursor_x;
        cursor_last_y = cursor_y;
        first_cursor = false;
    }

    camera.rotateCamera(delta_time, float(cursor_last_x - cursor_x), float(cursor_last_y - cursor_y));
    cursor_last_x = cursor_x;
    cursor_last_y = cursor_y;
    */
}