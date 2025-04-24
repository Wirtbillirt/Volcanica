#ifndef VOLCANO_H
#define VOLCANO_H

#include <glad/glad.h>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <stb_image.h>
#include <iostream>
#include <cmath>

class Volcano {
public:
    Volcano(const char* texturePath, float radiusBottom = 1.0f, float radiusTop = 0.4f,
            float height = 1.5f, int segments = 40) {
        generateMesh(radiusBottom, radiusTop, height, segments);
        texture = loadTexture(texturePath);
        setupBuffers();
    }

    ~Volcano() {
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
    void generateMesh(float radiusBottom, float radiusTop, float height, int segments) {
        vertices.clear();
        indices.clear();

        // Generar vértices para base y parte superior
        for (int i = 0; i <= segments; ++i) {
            float angle = 2.0f * glm::pi<float>() * i / segments;

            // Vértice base
            float xBase = cos(angle) * radiusBottom;
            float zBase = sin(angle) * radiusBottom;
            vertices.insert(vertices.end(), {xBase, 0.0f, zBase});
            vertices.push_back(static_cast<float>(i) / segments); // U
            vertices.push_back(0.0f); // V

            // Vértice superior
            float xTop = cos(angle) * radiusTop;
            float zTop = sin(angle) * radiusTop;
            vertices.insert(vertices.end(), {xTop, height, zTop});
            vertices.push_back(static_cast<float>(i) / segments); // U
            vertices.push_back(1.0f); // V
        }

        // Centro de la base
        int baseCenterIndex = vertices.size() / 8;
        vertices.insert(vertices.end(), {0.0f, 0.0f, 0.0f, 0.5f, 0.5f});

        // Generar índices para caras laterales y base
        for (int i = 0; i < segments; ++i) {
            int base1 = i * 2;
            int top1 = i * 2 + 1;
            int base2 = ((i + 1) % segments) * 2;
            int top2 = ((i + 1) % segments) * 2 + 1;

            // Caras laterales
            indices.insert(indices.end(), {base1, top1, base2});
            indices.insert(indices.end(), {top1, top2, base2});

            // Base
            indices.insert(indices.end(), {baseCenterIndex, base2, base1});
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

        // Posiciones (3 floats) + coordenadas de textura (2 floats)
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

#endif // VOLCANO_H