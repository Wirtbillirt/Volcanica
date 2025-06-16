#include "Collider.h"
#include <limits>

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
