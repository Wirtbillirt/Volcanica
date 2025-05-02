#ifndef SHADER_CLASS_H
#define SHADER_CLASS_H

#include <glad/glad.h>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cerrno>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

class Shader {
public:
    GLuint ID;

    // Constructor compila y enlaza los shaders
    Shader(const char* vertexPath, const char* fragmentPath);

    // Métodos principales
    void Activate();
    void Delete();

    // Métodos para uniforms
    void SetBool(const std::string& name, bool value) const;
    void SetInt(const std::string& name, int value) const;
    void SetFloat(const std::string& name, float value) const;
    void SetMat4(const std::string& name, const glm::mat4& mat) const;
    void SetVec3(const std::string& name, const glm::vec3& vec) const;

private:
    // Función para leer archivos de shaders
    static std::string get_file_contents(const char* filename);
};

#endif