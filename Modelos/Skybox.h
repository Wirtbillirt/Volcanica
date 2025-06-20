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
    std::vector<GLuint> cubemapTextures;
    int skyboxActual = 0;    
    Shader skyboxShader;

public:
    Skybox(); // en lugar de usar el constructor con ruta

    void CargarSkybox(const std::string& directory);
    void CambiarSkybox(int index);
    void Dibujar(Camera& camara);

    ~Skybox();
};
