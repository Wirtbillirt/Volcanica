#include "Collider.h"
#include <limits>
#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>


Collider::Collider()
    : localMin(0.0f), localMax(0.0f), worldMin(0.0f), worldMax(0.0f) {
}

Collider::Collider(const glm::vec3& minBounds, const glm::vec3& maxBounds)
    : localMin(minBounds), localMax(maxBounds), worldMin(minBounds), worldMax(maxBounds) {
}

void Collider::update(const glm::mat4& transform) {
    glm::vec3 corners[8] = {
        {localMin.x, localMin.y, localMin.z},
        {localMax.x, localMin.y, localMin.z},
        {localMin.x, localMax.y, localMin.z},
        {localMax.x, localMax.y, localMin.z},
        {localMin.x, localMin.y, localMax.z},
        {localMax.x, localMin.y, localMax.z},
        {localMin.x, localMax.y, localMax.z},
        {localMax.x, localMax.y, localMax.z},
    };

    worldMin = glm::vec3(std::numeric_limits<float>::max());
    worldMax = glm::vec3(std::numeric_limits<float>::lowest());

    for (const auto& corner : corners) {
        glm::vec3 transformed = glm::vec3(transform * glm::vec4(corner, 1.0f));
        worldMin = glm::min(worldMin, transformed);
        worldMax = glm::max(worldMax, transformed);
    }
}

bool Collider::intersects(const Collider& other) const {
    return (worldMin.x <= other.worldMax.x && worldMax.x >= other.worldMin.x) &&
        (worldMin.y <= other.worldMax.y && worldMax.y >= other.worldMin.y) &&
        (worldMin.z <= other.worldMax.z && worldMax.z >= other.worldMin.z);
}

glm::vec3 Collider::getMin() const {
    return worldMin;
}

glm::vec3 Collider::getMax() const {
    return worldMax;
}

void Collider::Draw(Shader& shader, Camera& camera) {
    // Validación: no dibujar si no se ha hecho update()
    if (worldMin == worldMax) return;

    glm::vec3 corners[8] = {
        {worldMin.x, worldMin.y, worldMin.z},
        {worldMax.x, worldMin.y, worldMin.z},
        {worldMax.x, worldMax.y, worldMin.z},
        {worldMin.x, worldMax.y, worldMin.z},
        {worldMin.x, worldMin.y, worldMax.z},
        {worldMax.x, worldMin.y, worldMax.z},
        {worldMax.x, worldMax.y, worldMax.z},
        {worldMin.x, worldMax.y, worldMax.z}
    };

    unsigned int indices[] = {
        0, 1, 1, 2, 2, 3, 3, 0, // base
        4, 5, 5, 6, 6, 7, 7, 4, // top
        0, 4, 1, 5, 2, 6, 3, 7  // verticals
    };

    GLuint VAO, VBO, EBO;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(corners), corners, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
    glEnableVertexAttribArray(0);

    shader.Activate();
    camera.updateMatrix(45.0f, 0.1f, 1000.0f);
    camera.Matrix(shader, "camMatrix");

    glDrawElements(GL_LINES, sizeof(indices) / sizeof(unsigned int), GL_UNSIGNED_INT, 0);

    glDeleteBuffers(1, &VBO);
    glDeleteBuffers(1, &EBO);
    glDeleteVertexArrays(1, &VAO);
}

