#ifndef MESH_H
#define MESH_H

#include <vector>
#include <string>
#include "VAO.h"
#include "VBO.h"
#include "EBO.h"
#include "Shader.h"

struct Vertex {
    glm::vec3 Position;
    glm::vec3 Normal;
    glm::vec2 TexCoords;
};

struct Texture {
    unsigned int id;
    std::string type;
    std::string path;
};

class Mesh {
public:
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<Texture> textures;
    VAO VAO;

    Mesh(std::vector<Vertex> vertices,
         std::vector<unsigned int> indices,
         std::vector<Texture> textures = {});
    void Draw(Shader& shader);

private:
    VBO VBO;
    EBO EBO;
    void setupMesh();
};

#endif