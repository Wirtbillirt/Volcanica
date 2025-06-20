#pragma once

#include "Skybox.h"
#include "Camara.h"
#include "imgui.h"
#include <vector>
#include <string>

class Interfaz
{
private:
    Skybox skybox;
    std::vector<std::string> rutasSkyboxes;
    int indiceActual = 0;

public:
    Interfaz();

    // Agrega una carpeta de skybox (debe contener right.jpg, left.jpg, etc.)
    void AgregarSkybox(const std::string& carpeta);

    // Llama esta función dentro de la escena para mostrar el botón y dibujar el skybox
    void Dibujar(Camera& camara);

    // Cambiar manualmente a un índice específico
    void SetSkybox(int index);
};
