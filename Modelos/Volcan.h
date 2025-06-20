#pragma once

#include "Model.h"
#include "shaderClass.h"
#include "Camara.h"
#include "AudioManager.h"
#include <string>
#include <vector>
#include <glm/glm.hpp>
#include <glm/gtc/quaternion.hpp>
#include "Skybox.h"
#include "Interfaz.h"

class Volcan
{
private:
	int width, height;
	Model* model = nullptr;                              // Modelo principal
	std::vector<Model*> modelosExtra;                    // Modelos adicionales
	std::vector<glm::vec3> posicionesModelosExtra;       // Posiciones de cada modelo extra
	std::vector<glm::vec3> rotacionesModelosExtra;
	std::vector<glm::vec3> escalasModelosExtra;
	std::string rutaModelo;

	AudioManager audio;
	Interfaz interfazSkybox;

	bool colisionDetectada = false;

public:
	Volcan(int width, int height);
	~Volcan();

	void CargarModelo(int volcanSeleccionado);
	void Dibujar(Shader& shader, Camera& camara);
};
