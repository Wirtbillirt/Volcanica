#include "VBO.h"

VBO::VBO(float* vertices, GLsizeiptr size) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO_ID);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_ID);
    glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);
}

void VBO::Bind() {
    glBindVertexArray(VAO);
}

void VBO::Unbind() {
    glBindVertexArray(0);
}

void VBO::Delete() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO_ID);
}
