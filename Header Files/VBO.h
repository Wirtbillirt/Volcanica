#ifndef VBO_H
#define VBO_H

#include <glad/glad.h>

class VBO {
public:
    GLuint VAO, VBO_ID;

    VBO(float* vertices, GLsizeiptr size);

    void Bind();
    void Unbind();
    void Delete();
};

#endif
