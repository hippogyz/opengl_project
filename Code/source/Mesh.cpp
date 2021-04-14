#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include "Mesh.h"
#include "Shader.h"

Mesh::Mesh(std::vector< Vertex > vertices, std::vector< unsigned int > indices, std::vector<Texture>textures)
{
	this->vertices = vertices;
	this->indices = indices;
	this->textures = textures;

	initialize_mesh();
}

void Mesh::initialize_mesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);
    // bind VAO
    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * vertices.size(), &vertices[0], GL_DYNAMIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned int) * indices.size(), &indices[0], GL_DYNAMIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_TRUE, 14 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 14 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // reset opengl state
    glBindVertexArray(0);
}

void Mesh::Draw(Shader& shader)
{
    // load textures
    shader.useShader();
    int textureOrder = 0;
    for (auto it = textures.begin(); it != textures.end(); ++it)
    {
        Texture texture = *it;
        switch (texture.type)
        {
            case DIFFUSE_TEXT:
                shader.setInt("material.diffuse_texture", textureOrder);
                break;
            case SPECULAR_TEXT:
                shader.setInt("material.diffuse_texture", textureOrder);
                break;
            case NORMAL_TEXT:
                shader.setInt("material.normal_texture", textureOrder);
                break;
            case HEIGHT_TEXT:
                shader.setInt("material.height_texture", textureOrder);
                break;
        }
        glActiveTexture(GL_TEXTURE0 + textureOrder);
        glBindTexture(GL_TEXTURE_2D, texture.id);

        ++textureOrder;
    }

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, (void*)0);

    // reset opengl state
    glBindVertexArray(0);
    glActiveTexture(GL_TEXTURE0);
}
