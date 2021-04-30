#include "Shader.h"

Shader::Shader()
{
	prog = glCreateProgram();
    shader_hash = 0;

	if (!prog)
	{
		printf("--- \"Shader.h\" Shader() : failed to create program --- /n ");
	}
}

Shader::~Shader() 
{
    std::cout << "shader " << prog << " destroyed" << std::endl;
	glDeleteProgram(prog);
}

void Shader::shaderAttachFromFile(GLenum shader_type, const char* shader_path)
{
	unsigned int shader = compileShaderFromFile(shader_type, shader_path);
	if (shader)
	{
		glAttachShader(prog, shader);
		glDeleteShader(shader);
	}
}

void Shader::linkShaderProgram()
{
	glLinkProgram(prog);

    int prog_success;
    glGetProgramiv(prog, GL_LINK_STATUS, &prog_success);
    if (!prog_success)
    {
        char infoLog[512];
        glGetProgramInfoLog(prog, 512, NULL, infoLog);
        printf(" --- \"Shader.h\" linkShaderProgram(): failed to link program for shaders --- \n");
    }
}

void Shader::initializeShaderVF(const char* vs_path, const char* fs_path)
{
	shaderAttachFromFile(GL_VERTEX_SHADER, vs_path);
	shaderAttachFromFile(GL_FRAGMENT_SHADER,  fs_path);
	linkShaderProgram();

    shader_hash = std::hash< std::string >() (std::string(vs_path) + std::string(fs_path));
}

void Shader::useShader() 
{
	glUseProgram(prog);
}

void Shader::setFloat(const char* name, float f1)
{
    int location = glGetUniformLocation(prog, name);
    glUniform1f(location, f1);
}

void Shader::setFloat(const char* name, float f1, float f2)
{
    int location = glGetUniformLocation(prog, name);
    glUniform2f(location, f1, f2);
}

void Shader::setFloat(const char* name, float f1, float f2, float f3)
{
    int location = glGetUniformLocation(prog, name);
    glUniform3f(location, f1, f2, f3);
}

void Shader::setFloat(const char* name, float f1, float f2, float f3, float f4)
{
    int location = glGetUniformLocation(prog, name);
    glUniform4f(location, f1, f2, f3, f4);
}


void Shader::setInt(const char* name, int i1)
{
    int location = glGetUniformLocation(prog, name);
    glUniform1i(location, i1);
}


void Shader::setBool(const char* name, bool b1)
{
    setInt(name, (int)b1);
}

void Shader::setVec3(const char* name, glm::vec3& vec) 
{
    setFloat(name, vec.x, vec.y, vec.z);
}

void Shader::setTrans(const char* name, glm::mat4& trans)
{
    int location = glGetUniformLocation(prog, name);
    glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(trans));
}

char* Shader::loadShaderFromFile(const char* shader_path)
{
    const size_t buf_len = 512;

    FILE* fp;
    fopen_s(&fp, shader_path, "r"); // for vs
    if (!fp)
    {
        printf(" --- \"Shader.h\" loadShader: failed to open shader file at path: %s ---  \n", shader_path);
        return nullptr;
    }

    char buf[buf_len];
    char* source = nullptr;
    size_t current_len = 0;
    size_t read_len = 0;

    while ((read_len = fread(buf, 1, buf_len, fp)) > 0)
    {
        char* temp = (char*)malloc(current_len + read_len + 1);
        if (!temp)
        {
            printf(" --- \"Shader.h\" loadShader: failed to malloc temp buffer with size: %d --- \n", int(current_len + read_len + 1));
            if (source)
            {
                free(source);
            }
            fclose(fp);
            return nullptr;
        }

        if (source)
        {
            memcpy(temp, source, current_len);
            free(source);
        }
        memcpy(temp + current_len, buf, read_len);
        source = temp;
        current_len += read_len;
    }

    if (source)
    {
        source[current_len] = '\0';
    }

    fclose(fp);
    return source;
}

unsigned int Shader::compileShaderFromFile(GLenum shader_type, const char* shader_path)
{
    char* shader_source = loadShaderFromFile(shader_path);
    if (!shader_source)
    {
        printf(" --- \"Shader.h\" compileShader: failed to load shader at path: %s --- \n", shader_path);
        return 0;
    }

    unsigned int shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, (const char**)&shader_source, NULL);
    glCompileShader(shader);
    free(shader_source);

    //check compile status
    int compile_success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_success);
    if (!compile_success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf(" --- \"Shader.h\" compileShader: failed to compile shader at path: %s \n with info: %s --- \n", shader_path, infoLog);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}