#include "RenderManager.h"

#include <iostream>
#include "Game.h"

const unsigned int RenderManager::SCR_WIDTH = 600;
const unsigned int RenderManager::SCR_HEIGHT = 600;

RenderManager::RenderManager()
{
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
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, cursor_move_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED); // fix cursor for First Personal Sight

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        glfwTerminate();
    }

    // enable depth test
    // ------------------
    glEnable(GL_DEPTH_TEST);
}

void RenderManager::BeforeRender()
{
    clearBuffer(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


    // update camera

}

void RenderManager::AfterRender()
{
    // nothing to do now
}




void RenderManager::clearBuffer(GLenum config)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
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