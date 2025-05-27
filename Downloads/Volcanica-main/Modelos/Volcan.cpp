#include "Volcan.h"
#include <iostream>

Volcan::Volcan(int width, int height)
{
	this->width = width;
	this->height = height;
}

Volcan::~Volcan()
{
	if (model != nullptr)
		delete model;
}

void Volcan::CargarModelo(int volcanSeleccionado)
{
	if (model != nullptr)
	{
		delete model;
		model = nullptr;
	}

	switch (volcanSeleccionado)
	{
	case 1:
		rutaModelo = "models/volcan1/scene.gltf";

		audio.playSoundEffect("assets/music/audio2.wav");
		audio.setEffectVolume(0.5f);
		
		break;
	case 2:
		rutaModelo = "models/volcan2/scene.gltf";
		break;
	case 3:
		rutaModelo = "models/volcan3/scene.gltf";
		break;
	default:
		std::cerr << "Volcán no válido\n";
		return;
	}

	model = new Model(rutaModelo.c_str());
	std::cout << "Modelo cargado: " << rutaModelo << std::endl;
}

void Volcan::Dibujar(Shader& shader, Camera& camara)
{
	if (model)
		model->Draw(shader, camara);
}
