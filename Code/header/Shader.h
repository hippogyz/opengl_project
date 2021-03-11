#pragma once
#include <glad/glad.h>

#include <iostream>
#include <stdio.h>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
	unsigned int prog;
	
	Shader();
	~Shader();
	void useShader();
	void shaderAttachFromFile(GLenum shader_type, const char* shader_path);
	void linkShaderProgram();
	void initializeShaderVF(const char* vs_path, const char* fs_path);

	void setFloat(const char* name, float f1);
	void setFloat(const char* name, float f1, float f2);
	void setFloat(const char* name, float f1, float f2, float f3);
	void setFloat(const char* name, float f1, float f2, float f3, float f4);

	void setInt(const char* name, int i1);

	void setBool(const char* name, bool b1);

	void setVec3(const char* name, glm::vec3& vec);
	void setTrans(const char* name, glm::mat4& trans);


protected:
	char* loadShaderFromFile(const char* shader_path);
	unsigned int compileShaderFromFile(GLenum shader_type, const char* shader_path);
};