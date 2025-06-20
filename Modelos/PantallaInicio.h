#ifndef PANTALLA_INICIO_H
#define PANTALLA_INICIO_H
#include "AudioManager.h"

class PantallaInicio
{
public:
    // Estado para saber si el botón fue presionado
    bool iniciarPresionado = false;

    // Dimensiones de ventana
    int width;
    int height;
    AudioManager audio;

    PantallaInicio(int width, int height);

    // Llama al renderizado de la pantalla
    void Dibujar();
};

#endif
