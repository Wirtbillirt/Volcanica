#ifndef VAO_H
#define VAO_H

#include <glad/glad.h>
#include "VBO.h"

class VAO {
public:
    GLuint ID;  // ID de OpenGL para el VAO

    VAO();          // Constructor
    void LinkAttrib(VBO& VBO, GLuint layout, GLuint numComponents, GLenum type, GLsizeiptr stride, void* offset);  // Vincular atributos
    void Bind();    // Vincular VAO
    void Unbind();  // Desvincular VAO
    void Delete();  // Eliminar VAO
};

#endif