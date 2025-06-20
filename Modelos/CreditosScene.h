#pragma once
#include <imgui.h>

class CreditosScene {
public:
    CreditosScene(int width, int height);
    void Dibujar();
    bool regresarPresionado = false;

private:
    int width;
    int height;
    float tiempoAcumulado = 0.0f;
};
