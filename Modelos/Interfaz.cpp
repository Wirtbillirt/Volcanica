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
    ImGui::SetNextWindowPos(ImVec2(1100, 20), ImGuiCond_Always); 
    ImGui::SetNextWindowSize(ImVec2(160, 55), ImGuiCond_Always); 
    ImGui::Begin("Atardecer - Noche", nullptr, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove);

    if (ImGui::Button("Cambiar ciclo"))
    {
        indiceActual = (indiceActual + 1) % rutasSkyboxes.size();
        skybox.CambiarSkybox(indiceActual);
    }

    ImGui::End();

    skybox.Dibujar(camara);
}
