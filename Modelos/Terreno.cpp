#include "Terreno.h"
#include <stb/stb_image.h>
#include <iostream>
#include <glm/gtc/type_ptr.hpp>
#include <glm/glm.hpp>

// Nueva UV más grande (para que no se vea verde o borroso por estirado)
float vertices[] = {
    // posiciones         // normales         // texcoords (repetidos 10x)
    -50.0f, 0.0f, -50.0f,  0.0f, 1.0f, 0.0f,  0.0f, 0.0f,
     50.0f, 0.0f, -50.0f,  0.0f, 1.0f, 0.0f, 10.0f, 0.0f,
     50.0f, 0.0f,  50.0f,  0.0f, 1.0f, 0.0f, 10.0f,10.0f,
    -50.0f, 0.0f,  50.0f,  0.0f, 1.0f, 0.0f,  0.0f,10.0f
};

unsigned int indices[] = {
    0, 1, 2,
    2, 3, 0
};

Terreno::Terreno(int width, int height) : width(width), height(height) {
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    // posiciones
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    // normales
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
    glEnableVertexAttribArray(1);
    // texcoords
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
    glEnableVertexAttribArray(2);

    // Cargar textura
    glGenTextures(1, &textura);
    glBindTexture(GL_TEXTURE_2D, textura);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

    int texWidth, texHeight, nrChannels;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load("models/terreno.jpg", &texWidth, &texHeight, &nrChannels, 0);
    if (data) {
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, texWidth, texHeight, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);
        std::cerr << "✅ Textura del terreno cargada\n";
    }
    else {
        std::cerr << "❌ No se pudo cargar la textura del terreno\n";
    }
    stbi_image_free(data);
}

void Terreno::Dibujar(Shader& shader, Camera& camara) {
    shader.Activate();
    camara.updateMatrix(45.0f, 0.1f, 1000.0f);
    camara.Matrix(shader, "camMatrix");

    // Matriz modelo (opcionalmente podrías rotar o escalar aquí si lo necesitas)
    glm::mat4 model = glm::mat4(1.0f);
    glUniformMatrix4fv(glGetUniformLocation(shader.ID, "model"), 1, GL_FALSE, glm::value_ptr(model));

    // Activar textura y conectar al shader
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, textura);
    glUniform1i(glGetUniformLocation(shader.ID, "tex0"), 0);

    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
}

Terreno::~Terreno() {
    glDeleteVertexArrays(1, &VAO);
    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteTextures(1, &textura);
}
