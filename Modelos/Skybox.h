#pragma once

#include <string>
#include <vector>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include "shaderClass.h"
#include "Camara.h"

class Skybox
{
private:
    unsigned int skyboxVAO, skyboxVBO, skyboxEBO;
    unsigned int cubemapTexture;
    Shader skyboxShader;

public:
    Skybox(const std::string& directory);

    void Dibujar(Camera& camara);

    ~Skybox();
};
