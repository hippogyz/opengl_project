#include "RenderManager.h"

#include <iostream>
#include "Game.h"
#include "Model.h"
#include "Shader.h"
#include "Component/CameraComponent.h"
#include "Component/LightComponent.h"

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

    camera_position = glm::vec3(0.0, 0.0, 3.0);
    view = glm::lookAt(camera_position, camera_position + glm::vec3(0.0, 0.0, -1.0), glm::vec3(0.0, 1.0, 0.0));
    projection = glm::perspective(glm::radians(45.0f), float(window_size[0]) / float(window_size[1]), 0.1f, 100.0f);

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
    glfwGetWindowSize(window, &(window_size[0]), &(window_size[1]));
    
    // update camera
    std::shared_ptr<CameraComponent> current_camera = camera.lock();
    if (current_camera != nullptr)
    {
        camera_position = current_camera->get_position();
        view = current_camera->get_view();
    }

    // update projection
     projection = glm::perspective(glm::radians(45.0f), float(window_size[0]) / float(window_size[1]), 0.1f, 100.0f);
    
    // update light
     for (auto&& c_light : lights)
     {
         std::shared_ptr<LightComponent> current_light = c_light.lock();
         for (auto&& shader : shaders)
         {
             shader->useShader();
             current_light->light->setLight(*shader, current_light->light->light_type);
         }
     }

     lights.clear();

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

void RenderManager::assign_camera(std::weak_ptr<CameraComponent>& camera)
{
    std::shared_ptr<CameraComponent> pre_camera = this->camera.lock();
    if (pre_camera.get() != camera.lock().get())
    {
        if (pre_camera != nullptr)
        {
            pre_camera->first_update = false;
            pre_camera->is_active = false;
        }

        this->camera = camera;
    }
}

void RenderManager::clearBuffer(GLenum config)
{
    glClearColor(0.2f, 0.2f, 0.2f, 1.0f);
    glClear(config);
}
