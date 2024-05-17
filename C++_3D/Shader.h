#pragma once
#include <string>
#include <glm/glm.hpp>

class Shader
{
public:
    unsigned int ID;
    unsigned int vertexShader;
    unsigned int fragmentShader;

    Shader(const char* vertexPath, const char* fragmentPath);

    void Process(std::string vertexCode, std::string fragmentCode);
    void Use() const;


    void SetMatrix4(const std::string &name, const glm::mat4 &value) const;

    void SetInt(const std::string &name, int value) const;
    void SetVector3(const std::string& name, float x, float y, float z) const;
    void SetFloat(const std::string& name, float value) const;

    void SetVector3(const std::string& name, const glm::vec3& value) const;

private:
    std::string GetFileContent(const char* path);
};
