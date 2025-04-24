#ifndef CRATER_H
#define CRATER_H

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <stb_image.h>
#include <iostream>
#include <cmath>

class Crater {
public:
    Crater(const char* texturePath, float radius = 0.4f, float depth = 0.3f, int segments = 40) {
        generateMesh(radius, depth, segments);
        texture = loadTexture(texturePath);
        setupBuffers();
    }

    ~Crater() {
        glDeleteVertexArrays(1, &VAO);
        glDeleteBuffers(1, &VBO);
        glDeleteBuffers(1, &EBO);
    }

    void Draw(GLuint shaderProgram, glm::mat4 model) {
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "model"),
            1, GL_FALSE, glm::value_ptr(model));
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, texture);
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

private:
    void generateMesh(float radius, float depth, int segments) {
        vertices.clear();
        indices.clear();

        // Centro del cráter (ligeramente hundido)
        vertices.insert(vertices.end(), {0.0f, -depth, 0.0f, 0.5f, 0.5f});

        // Vértices del borde del cráter
        for (int i = 0; i <= segments; ++i) {
            float angle = 2.0f * glm::pi<float>() * i / segments;
            float x = cos(angle) * radius;
            float z = sin(angle) * radius;

            // Coordenadas de textura en forma circular
            float u = 0.5f + 0.5f * cos(angle);
            float v = 0.5f + 0.5f * sin(angle);

            vertices.insert(vertices.end(), {x, 0.0f, z, u, v});
        }

        // Conectar los triángulos
        for (int i = 1; i <= segments; ++i) {
            indices.insert(indices.end(), {0, i, (i % segments) + 1});
        }

        // Añadir base para mejor visualización
        int baseIndex = vertices.size() / 5;
        for (int i = 1; i <= segments; ++i) {
            int next = (i % segments) + 1;
            indices.insert(indices.end(), {i, next, baseIndex});
        }
    }

    unsigned int loadTexture(const char* path) {
        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(true);
        unsigned char* data = stbi_load(path, &width, &height, &nrChannels, 0);
        if (data) {
            GLenum format = nrChannels == 4 ? GL_RGBA : GL_RGB;
            glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
            glGenerateMipmap(GL_TEXTURE_2D);
        } else {
            std::cerr << "Error cargando textura: " << path << std::endl;
        }
        stbi_image_free(data);
        return textureID;
    }

    void setupBuffers() {
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(float), vertices.data(), GL_STATIC_DRAW);

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int), indices.data(), GL_STATIC_DRAW);

        // Atributos: posición (3) + coordenadas de textura (2)
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(float), (void*)(3 * sizeof(float)));
        glEnableVertexAttribArray(1);

        glBindVertexArray(0);
    }

    unsigned int VAO, VBO, EBO;
    unsigned int texture;
    std::vector<float> vertices;
    std::vector<unsigned int> indices;
};

#endif // CRATER_H