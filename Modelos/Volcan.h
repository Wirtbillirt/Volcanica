#pragma once

#include "Model.h"
#include "shaderClass.h"
#include "Camara.h"
#include "AudioManager.h"
#include <string>
#include "Skybox.h"
#include <glm/glm.hpp> // Asegúrate de incluir esto
#include <glm/gtc/quaternion.hpp>

class Volcan
{
private:
	int width, height;
	Model* model = nullptr;
	Model* modelExtra = nullptr;               // Modelo adicional para pruebas de colisión
	glm::vec3 modeloExtraPos = glm::vec3(0.0f, 0.0f, 0.0f);  // posición para que no colisione con el principal
	glm::quat modeloExtraRotacion;

	std::string rutaModelo;
	Skybox skybox;
	AudioManager audio;
	
	bool colisionDetectada = false;

public:
	Volcan(int width, int height);
	~Volcan();

	void CargarModelo(int volcanSeleccionado);
	void Dibujar(Shader& shader, Camera& camara);
};
