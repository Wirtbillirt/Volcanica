#ifndef VBO_H
#define VBO_H

#include <glad/glad.h>

class VBO {
public:
    GLuint ID;
    VBO();  // Constructor vacío
    void Data(void* vertices, GLsizeiptr size);  // Método para datos crudos
    void Bind();
    void Unbind();
    void Delete();
};

#endif