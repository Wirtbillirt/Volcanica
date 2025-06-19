#include "SeleccionVolcan.h"
#include <imgui.h>
#include <iostream>

SeleccionVolcan::SeleccionVolcan(int width, int height)
{
    this->width = width;
    this->height = height;
}

void SeleccionVolcan::Dibujar()
{
    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2((float)width, (float)height));

    ImGui::Begin("SeleccionVolcan",
        nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoSavedSettings
    );

    ImVec2 buttonSize = ImVec2(200, 60);
    float spacing = 20.0f;
    ImVec2 windowSize = ImGui::GetWindowSize();

    float totalHeight = (buttonSize.y * 3) + (spacing * 2);
    float startY = (windowSize.y - totalHeight) * 0.5f;

    float posX = (windowSize.x - buttonSize.x) * 0.5f;

    ImGui::SetCursorPos(ImVec2(posX, startY));
    if (ImGui::Button("Volcán 1", buttonSize)) volcanSeleccionado = 1;

    ImGui::SetCursorPos(ImVec2(posX, startY + buttonSize.y + spacing));
    if (ImGui::Button("Volcán 2", buttonSize)) volcanSeleccionado = 2;

    ImGui::SetCursorPos(ImVec2(posX, startY + 2 * (buttonSize.y + spacing)));
    if (ImGui::Button("Volcán 3", buttonSize)) volcanSeleccionado = 3;

    ImGui::End();
}
