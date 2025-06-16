#include "Volcan.h"
#include <iostream>

Volcan::Volcan(int width, int height)
	: width(width), height(height), skybox("skybox/")
{
	
	// Cargar modelo adicional para pruebas de colisión
	modelExtra = new Model("models/volcan3/scene.gltf");  // Puedes cambiar esto por cualquier otro modelo
}

Volcan::~Volcan()
{
	if (model != nullptr)
		delete model;

	if (modelExtra != nullptr)
		delete modelExtra;

	
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
	// Skybox
	skybox.Dibujar(camara);

	
	// Modelo del volcán
	if (model)
		model->Draw(shader, camara);

	// Modelo adicional para pruebas
	if (modelExtra)
	{
		auto& meshes = modelExtra->GetMeshes();
		for (size_t i = 0; i < meshes.size(); ++i)
		{
			glm::mat4 rotX = glm::rotate(glm::mat4(1.0f), glm::radians(180.0f), glm::vec3(1, 0, 0));
			glm::mat4 rotZ = glm::rotate(glm::mat4(1.0f), glm::radians(-90.0f), glm::vec3(0, 0, 1));
			glm::mat4 globalTransform = rotZ * rotX; // Igual que el que se aplica en Model.cpp

			glm::mat4 modelMatrix = glm::translate(globalTransform, modeloExtraPos); // con posición aplicada
			meshes[i].Draw(shader, camara, modelMatrix);

		}
	}

	if (model && modelExtra)
	{
		const auto& meshes1 = model->GetMeshes();       // Modelo principal
		const auto& meshes2 = modelExtra->GetMeshes();  // Modelo extra

		for (const auto& m1 : meshes1)
		{
			for (const auto& m2 : meshes2)
			{
				if (m1.collider.intersects(m2.collider))
				{
					std::cout << "⚠️ ¡Colisión detectada entre modelos!\n";
					break;
				}
			}
		}
	}


}
