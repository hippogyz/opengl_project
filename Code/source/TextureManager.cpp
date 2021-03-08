#include "TextureManager.h"
#include <string>

TextureManager::TextureManager()
{
	loaded_textures = 0;
    textures = nullptr;
}

TextureManager::~TextureManager()
{
    if (textures)
    {
        glDeleteTextures(loaded_textures, textures);
        delete textures;
    }
}

void TextureManager::loadJPGsFromPath(int num, const char* paths[])
{
    loaded_textures = ( num < 16 ) ? num : 16;
	std::cout << " prepare to load jpg numbers: " << loaded_textures << std::endl;

	if ( textures )
	{
		printf(" --- \"TextureManager.h\" loadJPGsFromPath: each manager's load function should only be called once --- \n ");
		return;
	}

	textures = new unsigned int[loaded_textures];
	glGenTextures(loaded_textures, textures);

    int texture_width, texture_height, texture_nrChannels;
	for (int i = 0; i < loaded_textures; ++i)
	{
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_set_flip_vertically_on_load(true);
        unsigned char* texture_data = stbi_load(paths[i], &texture_width, &texture_height, &texture_nrChannels, 0);
        if (texture_data && strlen((char*)(texture_data)) < 4 * 65536)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texture_width, texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else if (!texture_data)
        {
            printf(" --- - \"TextureManager.h\" loadJPGsFromPath: failed to load texture at path: %s--- \n", paths[i]);
        }
        else
        {
            printf(" --- - \"TextureManager.h\" loadJPGsFromPath: loaded texture at path %s--- is oversized with %d bytes \n",
                paths[i], int(strlen((char*)(texture_data))) );
        }
        stbi_image_free(texture_data);
	}
}

void TextureManager::loadPNGsFromPath(int num, const char* paths[])
{
    loaded_textures = (num < 16) ? num : 16;
    std::cout << " prepare to load png numbers: " << loaded_textures << std::endl;

    if (textures)
    {
        printf(" --- \"TextureManager.h\" loadPNGsFromPath: each manager's load function should only be called once --- \n ");
        return;
    }

    textures = new unsigned int[loaded_textures];
    glGenTextures(loaded_textures, textures);

    int texture_width, texture_height, texture_nrChannels;
    for (int i = 0; i < loaded_textures; ++i)
    {
        glBindTexture(GL_TEXTURE_2D, textures[i]);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        stbi_set_flip_vertically_on_load(true);
        unsigned char* texture_data = stbi_load(paths[i], &texture_width, &texture_height, &texture_nrChannels, 0);
        if (texture_data && strlen((char*)(texture_data)) < 4 * 65536)
        {
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture_width, texture_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, texture_data);
            glGenerateMipmap(GL_TEXTURE_2D);
        }
        else if (!texture_data)
        {
            printf(" --- - \"TextureManager.h\" loadPNGsFromPath: failed to load texture at path: %s--- \n", paths[i]);
        }
        else
        {
            printf(" --- - \"TextureManager.h\" loadPNGsFromPath: loaded texture at path %s--- is oversized with %d bytes \n",
                paths[i], int(strlen((char*)(texture_data))));
        }
        stbi_image_free(texture_data);
    }
}

void TextureManager::setShaderForTexture(Shader*shader)
{
    // set shader
    shader->useShader();
    const char* name1 = "texture";
    for (int i = 0; i < loaded_textures; i++)
    {
        char name[16] = "";
        strcat_s(name, name1);
        strcat_s(name, std::to_string(i).c_str());

        shader->setInt(name, i);
    }
}

void TextureManager::activeTextures()
{
    for (int i = 0; i < loaded_textures; i++)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, textures[i]);
    }
}

void TextureManager::activeSingleTexture(int location, int index)
{
    if (index < loaded_textures)
    {
        glActiveTexture(GL_TEXTURE0 + location);
        glBindTexture(GL_TEXTURE_2D, textures[index]);
    }
}
