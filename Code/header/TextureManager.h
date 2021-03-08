#pragma once
#include <glad/glad.h>

#include <iostream>
#include <stdio.h>
#include "stb_image.h"
#include "Shader.h"

class TextureManager {
public:
	TextureManager();
	~TextureManager();
	void loadJPGsFromPath(int num, const char* paths[]);
	void loadPNGsFromPath(int num, const char* paths[]); // change RGB -> RGBA
	void setShaderForTexture(Shader* shader);
	void activeTextures();
	void activeSingleTexture(int location, int index);


protected:
	int loaded_textures;
	unsigned int* textures;
};