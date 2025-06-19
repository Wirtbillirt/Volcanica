#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>
#include "shaderClass.h"
#include "Camara.h"

class Terreno {
private:
    unsigned int VAO, VBO, EBO, textura;
    int width, height;

public:
    Terreno(int width, int height);
    void Dibujar(Shader& shader, Camera& camara);
    ~Terreno();
};
