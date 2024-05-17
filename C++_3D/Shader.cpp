#include "Shader.h"

#include <iostream>
#include <sstream>
#include <glad/glad.h>

#include "fstream"

std::string Shader::GetFileContent(const char* path)
{
    try
    {
        std::ifstream shaderFile;
        shaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);

        shaderFile.open(path);
        std::stringstream shaderStream;
        shaderStream << shaderFile.rdbuf();
        shaderFile.close();
        return shaderStream.str();
    }
    catch (std::ifstream::failure& e)
    {
        std::cout << "ERROR : File can't be read" << e.what() << '\n';
    }
    return {};
}

Shader::Shader(const char* vertexPath, const char* fragmentPath)
{
    std::string vertexCode;
    std::string fragmentCode;

    vertexCode = GetFileContent(vertexPath);
    fragmentCode = GetFileContent(fragmentPath);

    Process(vertexCode,fragmentCode);

}

void Shader::Process(std::string vertexCode, std::string fragmentCode)
{
    const char* vertexShaderSource = vertexCode.c_str();
    // Vertex shader creation => Only geometry
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // Check if it works
    int success;
    char infoLog[512];
    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if(!success)
    {
        glGetShaderInfoLog(vertexShader, 512,NULL, infoLog);
        std::cout << "VERTEX SHADER : COMPILATION FAILED: \n" << infoLog << '\n';
    }


    const char* fragmentShaderSource = fragmentCode.c_str();
    // Fragment shader creation => Only visual
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Check if it works
    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(vertexShader, 512,NULL,infoLog);
        std::cout << "FRAGMENT SHADER : COMPILATION FAILED : \n" << infoLog << '\n';
    }

    // Attach shaders
    ID = glCreateProgram();
    glAttachShader(ID, vertexShader);
    glAttachShader(ID, fragmentShader);
    glLinkProgram(ID);

    // Check
    glGetProgramiv(ID, GL_LINK_STATUS, &success);
    if(!success)
    {
        glGetProgramInfoLog(ID, 512, NULL, infoLog);
        std::cout << "SHADER PROGRAM : LINK FAILED:\n" << infoLog << '\n';
    }

    // Don't forget to delete shaders !
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
}

void Shader::SetMatrix4(const std::string& name, const glm::mat4& value) const
{
    glUniformMatrix4fv(glGetUniformLocation(ID, name.c_str()), 1, GL_FALSE, &value[0][0]);
}

void Shader::Use() const
{
    glUseProgram(ID);
}

void Shader::SetInt(const std::string& name, int value) const
{
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}


void Shader::SetVector3(const std::string& name, float x, float y, float z) const
{
    glUniform3f(glGetUniformLocation(ID, name.c_str()), x, y, z);
}
void Shader::SetFloat(const std::string& name, float value) const
{
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetVector3(const std::string& name, const glm::vec3& value) const
{
    glUniform3fv(glGetUniformLocation(ID, name.c_str()), 1, &value[0]);
}