#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <memory>
#include "stb_image.h"

#include "Shader.h"
#include "TextureManager.h"
#include "Camera.h"
#include "Light.h"

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Game.h"
#include "GameObject.h"
#include "Component.h"
#include "Component/TransformComponent.h"


void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void cursor_move_callback(GLFWwindow* window, double cursor_x, double cursor_y);
void processInput(GLFWwindow* window);
void clearBuffer(GLenum config);
unsigned int  loadTextureFromPath(Shader shader, unsigned int texture_order, const char* texture_path); // has been replaced by class TextureManager


// settings
const unsigned int SCR_WIDTH = 600;
const unsigned int SCR_HEIGHT = 600;

// global variable
//      time
float delta_time = 0.0f;
float process_time = 0.0f;
float real_time = 0.0f;
//      cursor
bool first_cursor = true;
double cursor_last_x = 0.0;
double cursor_last_y = 0.0;
//      camera
Camera camera(glm::vec3(0.0f, 0.0f, 3.0f));

int main()
{
    GameObject gameobject;
    gameobject.test_gameObject();

    Game game;
    game.add_object(std::unique_ptr<GameObject>(new GameObject()));
    game.add_object(std::unique_ptr<GameObject>(new GameObject()));
    game.add_object(std::unique_ptr<GameObject>(new GameObject()));
    game.game_update(0.1);

    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // uncomment this statement to fix compilation on OS X
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "MyOpenGL", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
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
        return -1;
    }

    //use Shader class
    Shader shaderProg;
    shaderProg.initializeShaderVF("opengl_project/Code/shader/vertex_shader.vs", "opengl_project/Code/shader/frag_shader.fs");

    Shader refShaderProg;
    refShaderProg.initializeShaderVF("opengl_project/Code/shader/vertex_shader.vs", "opengl_project/Code/shader/ref_frag_shader.fs");

    // load texture
    // --------------
    //unsigned int texture = loadTextureFromPath(shaderProg, 0, "Resource/test_texture.jpg");
    const char* texture_path[] = {
        "opengl_project/Resource/test_texture.jpg",
        "opengl_project/Resource/test_texture2.jpg",
        "opengl_project/Resource/test_texture3.jpg",
        "opengl_project/Resource/test_specular_texture.jpg"
    };

    float material_ambient = 0.1;
    float material_shininess = 256.0;

    TextureManager texture_manager;
    texture_manager.loadJPGsFromPath(4, texture_path);

    // fragment uniform  (for lighting material)
    shaderProg.useShader();
    shaderProg.setInt("material.default_texture", 0);
    shaderProg.setInt("material.specular_texture", 1);
    shaderProg.setFloat("material.ambient", material_ambient);
    shaderProg.setFloat("material.shininess", material_shininess);
    //texture_manager.setShaderForTexture(&shaderProg);


    // lighting    
    const int point_light_num = 4;
    glm::vec3 light_position[point_light_num] = {
        glm::vec3(5.0,  2.0, 1.0),
        glm::vec3(-2.0, 3.0, -10.0),
        glm::vec3(4.0, -9.0, 2.0),
        glm::vec3(-10.0, 2.0, -3.0)
    };

    PointLight point_light[point_light_num] = {
        PointLight(true, glm::vec3(0.7), glm::vec3(1.0), light_position[0]),
        PointLight(true, glm::vec3(0.7), glm::vec3(1.0), light_position[1]),
        PointLight(true, glm::vec3(0.7), glm::vec3(1.0), light_position[2]),
        PointLight(true, glm::vec3(0.7), glm::vec3(1.0), light_position[3])
    };

    DirLight dir_light = DirLight(true, glm::vec3(0.3), glm::vec3(0.5), glm::vec3(1.0, 1.0, -1.0) );
    
    glm::vec3 spot_light_position = glm::vec3(3.0);
    glm::vec3 spot_light_direction = glm::vec3(-1.0);
    SpotLight spot_light = SpotLight(true, glm::vec3(0.7), glm::vec3(1.0), spot_light_position, spot_light_direction);
    
    // enable light
    for (int i = 0; i < point_light_num; ++i)
    {
        std::string pl_name = std::string("point_light[") + char(i + int('0')) + "]";
        point_light[i].setLight(shaderProg, pl_name.c_str());
    }
    dir_light.setLight(shaderProg, "dir_light");
    spot_light.setLight(shaderProg, "spot_light");

    // enable depth test
    // ------------------
    glEnable(GL_DEPTH_TEST);

    // load vertex array
    // --------------
    float vertices[] = {
        -0.5f, -0.5f, -0.5f, 0.0, 0.0, -1.0,  0.0f, 0.0f,
         0.5f, -0.5f, -0.5f, 0.0, 0.0, -1.0,  1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 0.0, 0.0, -1.0,  1.0f, 1.0f,
        -0.5f,  0.5f, -0.5f, 0.0, 0.0, -1.0,  0.0f, 1.0f,

        -0.5f, -0.5f,  0.5f, 0.0, 0.0, 1.0,  0.0f, 0.0f,
         0.5f, -0.5f,  0.5f, 0.0, 0.0, 1.0,  1.0f, 0.0f,
         0.5f,  0.5f,  0.5f, 0.0, 0.0, 1.0,  1.0f, 1.0f,
        -0.5f,  0.5f,  0.5f, 0.0, 0.0, 1.0,  0.0f, 1.0f,

        -0.5f,  0.5f,  0.5f, -1.0, 0.0, 0.0,  1.0f, 0.0f,
        -0.5f,  0.5f, -0.5f, -1.0, 0.0, 0.0, 1.0f, 1.0f,
        -0.5f, -0.5f, -0.5f, -1.0, 0.0, 0.0, 0.0f, 1.0f,
        -0.5f, -0.5f,  0.5f, -1.0, 0.0, 0.0, 0.0f, 0.0f,

         0.5f,  0.5f,  0.5f, 1.0, 0.0, 0.0, 1.0f, 0.0f,
         0.5f,  0.5f, -0.5f, 1.0, 0.0, 0.0, 1.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 1.0, 0.0, 0.0, 0.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 1.0, 0.0, 0.0, 0.0f, 0.0f,

        -0.5f, -0.5f, -0.5f, 0.0, -1.0, 0.0, 0.0f, 1.0f,
         0.5f, -0.5f, -0.5f, 0.0, -1.0, 0.0, 1.0f, 1.0f,
         0.5f, -0.5f,  0.5f, 0.0, -1.0, 0.0, 1.0f, 0.0f,
        -0.5f, -0.5f,  0.5f, 0.0, -1.0, 0.0, 0.0f, 0.0f,

        -0.5f,  0.5f, -0.5f, 0.0, 1.0, 0.0, 0.0f, 1.0f,
         0.5f,  0.5f, -0.5f, 0.0, 1.0, 0.0, 1.0f, 1.0f,
         0.5f,  0.5f,  0.5f, 0.0, 1.0, 0.0, 1.0f, 0.0f,
        -0.5f,  0.5f,  0.5f, 0.0, 1.0, 0.0, 0.0f, 0.0f
    };

    unsigned int indices[] = {
        0, 1, 2,
        0, 2, 3,

        4, 5, 6,
        4, 6, 7,
        
        8, 9, 10,
        8, 10, 11,
        
        12, 13, 14,
        12, 14, 15,
        
        16, 17, 18,
        16, 18, 19,

        20, 21, 22,
        20, 22, 23
    };
        
    glm::vec3 cubePositions[] = {
        glm::vec3(0.0f,  0.0f,  0.0f),
        glm::vec3(2.0f,  5.0f, -15.0f),
        glm::vec3(-1.5f, -2.2f, -2.5f),
        glm::vec3(-3.8f, -2.0f, -12.3f),
        glm::vec3(2.4f, -0.4f, -3.5f),
        glm::vec3(-1.7f,  3.0f, -7.5f),
        glm::vec3(1.3f, -2.0f, -2.5f),
        glm::vec3(1.5f,  2.0f, -2.5f),
        glm::vec3(1.5f,  0.2f, -1.5f),
        glm::vec3(-1.3f,  1.0f, -1.5f)
    };

    unsigned int VAO,VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind VAO
    glBindVertexArray(VAO);
    
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // render loop
    // -----------
    real_time = glfwGetTime();

    while (!glfwWindowShouldClose(window))
    {
        // update time
        float pre_time = real_time;
        real_time = glfwGetTime();
        delta_time = (real_time - pre_time < 0.05f) ? real_time - pre_time : 0.05f;
        process_time += delta_time;

        // input
        processInput(window);

        double mouse_position[2] = { cursor_last_x, cursor_last_y };
        int window_size[2];
        glfwGetWindowSize(window, &(window_size[0]), &(window_size[1]));

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 norm_mat = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        glm::mat4 projection = glm::mat4(1.0f);
        model = glm::rotate(model, glm::radians(-55.0f), glm::vec3(1.0f, 0.0f, 0.0f));
        model = glm::rotate(model, glm::radians(process_time * 50.0f), glm::vec3(0.5f, 1.0f, 0.0f));
        norm_mat = glm::transpose(glm::inverse(model));
        view = camera.getViewMat();
        projection = glm::perspective(glm::radians(45.0f), float(window_size[0]) / float(window_size[1]), 0.1f, 100.0f);

        glm::vec3 camera_position = camera.getPosition();

        // render part
        clearBuffer(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        
        shaderProg.useShader();

        // vertex uniform
        shaderProg.setTrans("model", model);
        shaderProg.setTrans("norm_mat", norm_mat);
        shaderProg.setTrans("view", view);
        shaderProg.setTrans("projection", projection);

        // fragment uniform
        shaderProg.setFloat("u_time", process_time);
        shaderProg.setFloat("u_mouse", float(mouse_position[0]), float(window_size[1]) - float(mouse_position[1]));
        shaderProg.setFloat("u_resolution", float(window_size[0]), float(window_size[1]));

        shaderProg.setFloat("camera_position", camera_position.x, camera_position.y, camera_position.z);
        //texture_manager.activeTextures();
        //glActiveTexture(GL_TEXTURE0);
        //glBindTexture(GL_TEXTURE_2D, texture);

        glBindVertexArray(VAO);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
        
        for (int i = 0; i < 10; ++i)
        {
            glm::mat4 model2 = glm::mat4(1.0f);
            glm::mat4 norm_mat2 = glm::mat4(1.0f);
            model2 = glm::translate(model2, cubePositions[i]);
            model2 = glm::rotate(model2, glm::radians(20.0f * i), glm::vec3(1.0f, 0.3f, 0.5f));
            model2 = glm::rotate(model2, glm::radians(process_time * 5.0f * i), glm::vec3(0.5f, 1.0f, 0.0f));
            norm_mat2 = glm::transpose(glm::inverse(model2));

            shaderProg.setTrans("model", model2);
            shaderProg.setTrans("norm_mat", norm_mat2);

            texture_manager.activeSingleTexture(1, 3);

            texture_manager.activeSingleTexture(0, 0);
            glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void*)0);
            texture_manager.activeSingleTexture(0, 1);
            glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void*)(12 * sizeof(unsigned int)));
            texture_manager.activeSingleTexture(0, 2);
            glDrawElements(GL_TRIANGLES, 12, GL_UNSIGNED_INT, (void*)(24 * sizeof(unsigned int)));
        }

        // draw light source

        refShaderProg.useShader();
        refShaderProg.setTrans("view", view);
        refShaderProg.setTrans("projection", projection);

        refShaderProg.setFloat("u_time", process_time);
        refShaderProg.setFloat("u_mouse", float(mouse_position[0]), float(window_size[1]) - float(mouse_position[1]));
        refShaderProg.setFloat("u_resolution", float(window_size[0]), float(window_size[1]));

        for (int i = 0; i < point_light_num; ++i)
        {
            if (point_light[i].is_active)
            {
                glm::mat4 ref_model = glm::mat4(1.0f);
                glm::mat4 ref_norm_mat = glm::mat4(1.0f);
                ref_model = glm::translate(ref_model, glm::vec3(light_position[i].x, light_position[i].y, light_position[i].z));
                ref_model = glm::scale(ref_model, glm::vec3(0.3));
                ref_norm_mat = glm::transpose(glm::inverse(ref_model));

                refShaderProg.useShader();
                refShaderProg.setTrans("model", ref_model);
                refShaderProg.setTrans("norm_mat", ref_norm_mat);

                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);
            }
        }

        if (spot_light.is_active)
        {
            glm::mat4 ref_spot_model = glm::mat4(1.0f);
            glm::mat4 ref_spot_norm_mat = glm::mat4(1.0f);
            ref_spot_model = glm::translate(ref_spot_model, glm::vec3(spot_light_position.x, spot_light_position.y, spot_light_position.z));
            ref_spot_model = glm::scale(ref_spot_model, glm::vec3(0.4));
            ref_spot_norm_mat = glm::transpose(glm::inverse(ref_spot_model));

            refShaderProg.useShader();
            refShaderProg.setTrans("model", ref_spot_model);
            refShaderProg.setTrans("norm_mat", ref_spot_norm_mat);

            glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);

            for (int i = 0; i < 5; ++i)
            {
                glm::mat4 ref_model = glm::mat4(1.0f);
                glm::mat4 ref_norm_mat = glm::mat4(1.0f);
                glm::vec3 spot_dir = spot_light_position + glm::normalize(spot_light_direction) * float(0.1f * i + 0.4f);
                ref_model = glm::translate(ref_model, glm::vec3(spot_dir.x, spot_dir.y, spot_dir.z));
                ref_model = glm::scale(ref_model, glm::vec3(0.2));
                ref_norm_mat = glm::transpose(glm::inverse(ref_model));

                refShaderProg.useShader();
                refShaderProg.setTrans("model", ref_model);
                refShaderProg.setTrans("norm_mat", ref_norm_mat);

                glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, (void*)0);
            }
        }



        //glDrawArrays(GL_TRIANGLES, 0, 3);


        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glfwTerminate();
    return 0;
}


//---------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------
//---------------------------------------------------------------------------------------------------------------------------------------------------------

// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
        camera.moveCamera(delta_time, MOVE_FORWARD);

    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
        camera.moveCamera(delta_time, MOVE_BACKWARD);

    if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
        camera.moveCamera(delta_time, MOVE_RIGHT);

    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
        camera.moveCamera(delta_time, MOVE_LEFT);
}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and 
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}

void cursor_move_callback(GLFWwindow* window, double cursor_x, double cursor_y)
{
    if (first_cursor)
    {
        cursor_last_x = cursor_x;
        cursor_last_y = cursor_y;
        first_cursor = false;
    }

    camera.rotateCamera(delta_time, float(cursor_last_x - cursor_x ), float(cursor_last_y - cursor_y));
    cursor_last_x = cursor_x;
    cursor_last_y = cursor_y;
}

void clearBuffer(GLenum config)
{
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glClear(config);
}

unsigned int  loadTextureFromPath(Shader shader, unsigned int texture_order, const char* texture_path)
{
    unsigned int texture;
    glGenTextures(1, &texture);
    glBindTexture(GL_TEXTURE_2D, texture);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int texture_width, texture_height, texture_nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* texture_data = stbi_load(texture_path, &texture_width, &texture_height, &texture_nrChannels, 0);
    if (texture_data)
    {
        //std::cout << strlen((char*)(texture_data)) << std::endl;
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width, texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);
        glGenerateMipmap(GL_TEXTURE_2D);
    }
    else
    {
        printf(" --- loadTextureFromPath(): failed to load texture at path: %s --- \n", texture_path);
    }
    stbi_image_free(texture_data);

    shader.useShader();
    char uni_name[] = "texture0";
    uni_name[7] = char(texture_order + 48); // change the index with texture_order
    shader.setInt(uni_name, texture_order);

    return texture;
}