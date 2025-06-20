#ifndef SELECCION_VOLCAN_H
#define SELECCION_VOLCAN_H
#include "AudioManager.h"

class SeleccionVolcan
{
public:
    int volcanSeleccionado = 0; // 0 = ninguno, 1, 2, 3 = volcán elegido
    int width, height;

	AudioManager audio;

    SeleccionVolcan(int width, int height);
    void Dibujar();
};

#endif
