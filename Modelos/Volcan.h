#pragma once

#include "Model.h"
#include "shaderClass.h"
#include "Camara.h"
#include "AudioManager.h"
#include <string>

class Volcan
{
private:
	int width, height;
	Model* model = nullptr;
	std::string rutaModelo;

	AudioManager audio;

public:
	Volcan(int width, int height);
	~Volcan();

	void CargarModelo(int volcanSeleccionado);
	void Dibujar(Shader& shader, Camera& camara);
};
