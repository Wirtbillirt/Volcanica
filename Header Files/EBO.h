#ifndef EBO_H
#define EBO_H

#include <glad/glad.h>
#include <vector>

class EBO {
public:
    GLuint ID;

    EBO();  // Constructor vacío
    void Data(void* indices, GLsizeiptr size);  // Método para enviar datos
    void Bind();
    void Unbind();
    void Delete();
};

#endif