/*
    ---- This file has been abandoned ----
*/

#include "shaderReader.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>

#include <iostream>
#include <stdio.h>

static char* loadShaderFromFile(const char* shader_path)
{
    const size_t buf_len = 512;

    FILE* fp;
    fopen_s(&fp, shader_path, "r"); // for vs
    if (!fp)
    {
        printf(" --- \"shaderReader.h\" loadShader: failed to open shader file at path: %s ---  \n", shader_path);
        return nullptr;
    }

    char buf[ buf_len ];
    char* source = nullptr;
    size_t current_len = 0;
    size_t read_len = 0;

    while ((read_len = fread(buf, 1, buf_len, fp)) > 0)
    {
        char* temp = (char*)malloc(current_len + read_len + 1);
        if (!temp)
        {
            printf(" --- \"shaderReader.h\" loadShader: failed to malloc temp buffer with size: %d --- \n", int(current_len + read_len + 1));
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

static unsigned int compileShaderFromFile(GLenum shader_type, const char* shader_path)
{
    char* shader_source = loadShaderFromFile(shader_path);
    if (!shader_source)
    {
        printf(" --- \"shaderReader.h\" compileShader: failed to load shader at path: %s --- \n", shader_path);
        return 0;
    }

    unsigned int shader = glCreateShader(shader_type);
    glShaderSource(shader, 1, (const char **)&shader_source, NULL);
    glCompileShader(shader);
    free(shader_source);

    //check compile status
    int compile_success;
    glGetShaderiv(shader, GL_COMPILE_STATUS, &compile_success);
    if (!compile_success)
    {
        char infoLog[512];
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        printf(" --- \"shaderReader.h\" compileShader: failed to compile shader at path: %s \n with info: %s --- \n", shader_path, infoLog);
        glDeleteShader(shader);
        return 0;
    }

    return shader;
}

void shaderAttachFromFile(const unsigned int gl_program, GLenum shader_type, const char* shader_path)
{
    unsigned int shader = compileShaderFromFile(shader_type, shader_path);
    if (shader)
    {
        glAttachShader(gl_program, shader);
        glDeleteShader(shader);
    }
}
