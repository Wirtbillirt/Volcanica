#include "CreditosScene.h"
#include <imgui.h>
#include <cmath>

CreditosScene::CreditosScene(int width, int height)
    : width(width), height(height), tiempoAcumulado(0.0f) {
}

void CreditosScene::Dibujar() {
    float dt = ImGui::GetIO().DeltaTime;
    tiempoAcumulado += dt;

    ImGui::SetNextWindowPos(ImVec2(0, 0));
    ImGui::SetNextWindowSize(ImVec2((float)width, (float)height));

    // Fondo negro
    ImGui::PushStyleColor(ImGuiCol_WindowBg, ImVec4(0.f, 0.f, 0.f, 1.f));

    ImGui::Begin("Creditos", nullptr,
        ImGuiWindowFlags_NoTitleBar |
        ImGuiWindowFlags_NoResize |
        ImGuiWindowFlags_NoMove |
        ImGuiWindowFlags_NoCollapse |
        ImGuiWindowFlags_NoScrollbar |
        ImGuiWindowFlags_NoSavedSettings);

    ImVec2 windowSize = ImGui::GetWindowSize();
    float centerX = windowSize.x * 0.5f;

    // Escala de fuente grande
    ImGui::SetWindowFontScale(2.5f);

    // Cr�ditos
    const char* textos[] = {
        "Universidad Nacional De Ingenieria - 2025",
        "",
        "Proyecto Volcanica",
        "",
        "DESARROLLADO POR",
        "",

        "Wilberth Alejandro Perez Loredo",
        "",
        "Oscar Moises Perez Delgado",
        "",
        "Brandon Brandy Downs Hebberth.",
        "",
        "Angel Rafael Maltez Navarrete.",

    };

    const int numLineas = sizeof(textos) / sizeof(textos[0]);

    float lineHeight = ImGui::GetTextLineHeightWithSpacing();
    float totalTextHeight = numLineas * lineHeight;

    // Velocidad constante de desplazamiento (p�xeles por segundo)
    float velocidad = 200.0f;

    // Posici�n vertical inicial (desde abajo hacia arriba)
    float offsetY = windowSize.y - fmod(tiempoAcumulado * velocidad, totalTextHeight + windowSize.y);

    // Dibujar cada l�nea en su posici�n relativa
    for (int i = 0; i < numLineas; i++) {
        float posY = offsetY + i * lineHeight;

        // Solo dibujar si est� dentro de la pantalla
        if (posY + lineHeight < 0 || posY > windowSize.y)
            continue;

        float textWidth = ImGui::CalcTextSize(textos[i]).x;
        float posX = centerX - textWidth * 0.5f;

        ImGui::SetCursorPos(ImVec2(posX, posY));
        ImGui::Text("%s", textos[i]);
    }

    // Restaurar escala normal para el bot�n
    ImGui::SetWindowFontScale(1.0f);
    ImGui::SetCursorPos(ImVec2(20, 20));
    if (ImGui::Button("Regresar", ImVec2(100, 40))) {
        regresarPresionado = true;
    }

    ImGui::End();
    ImGui::PopStyleColor();
}
