#include "Interfaz.h"
#include <imgui.h>

Interfaz::Interfaz()
{
    // Espera a que se agreguen skyboxes manualmente
}

void Interfaz::AgregarSkybox(const std::string& carpeta)
{
    rutasSkyboxes.push_back(carpeta);
    skybox.CargarSkybox(carpeta);
}

void Interfaz::SetSkybox(int index)
{
    if (index >= 0 && index < rutasSkyboxes.size())
    {
        indiceActual = index;
        skybox.CambiarSkybox(index);
    }
}

void Interfaz::Dibujar(Camera& camara)
{
    ImGui::Begin("Dia - Noche");

    if (ImGui::Button("Cambiar Skybox"))
    {
        indiceActual = (indiceActual + 1) % rutasSkyboxes.size();
        skybox.CambiarSkybox(indiceActual);
    }

    ImGui::Text("Skybox actual: %d", indiceActual);
    ImGui::End();

    skybox.Dibujar(camara);
}
