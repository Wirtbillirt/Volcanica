#ifndef COLLIDER_H
#define COLLIDER_H

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <vector>

class Collider {
public:
    Collider();
    Collider(const glm::vec3& minBounds, const glm::vec3& maxBounds);

    void update(const glm::mat4& transform);
    bool intersects(const Collider& other) const;

    glm::vec3 getMin() const;
    glm::vec3 getMax() const;

private:
    glm::vec3 localMin;
    glm::vec3 localMax;
    glm::vec3 worldMin;
    glm::vec3 worldMax;
};

#endif
