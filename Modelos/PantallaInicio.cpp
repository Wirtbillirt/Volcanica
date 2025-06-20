#include "PantallaInicio.h"
#include "AudioManager.h"

#include <imgui.h>
#include <iostream>

PantallaInicio::PantallaInicio(int width, int height)
{
    this->width = width;
    this->height = height;
    iniciarPresionado = false;
}

void PantallaInicio::Dibujar()
{
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2((float)width, (float)height));

    ImGui::Begin("PantallaInicio",
        nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoSavedSettings |
        ImGuiWindowFlags_NoBringToFrontOnFocus
    );

    ImVec2 buttonSize = ImVec2(150, 50);
    ImVec2 windowSize = ImGui::GetWindowSize();
    ImVec2 buttonPos = ImVec2(
        (windowSize.x - buttonSize.x) * 0.5f,
        (windowSize.y - buttonSize.y) * 0.5f
    );

    ImGui::SetCursorPos(buttonPos);
    if (ImGui::Button("Iniciar", buttonSize))
    {
        audio.setEffectVolume(0.5f);
		audio.playSoundEffect("assets/effect/click.wav");
        iniciarPresionado = true;
        //std::cout << "¡Botón Iniciar presionado!" << std::endl;
    }

    ImGui::End();
}
