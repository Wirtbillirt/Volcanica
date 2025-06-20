#include "Volcan.h"
#include "ShaderCollider.h"
#include <iostream>

Volcan::Volcan(int width, int height)
	: width(width), height(height)
{
	interfazSkybox.AgregarSkybox("skybox/");
	interfazSkybox.AgregarSkybox("skybox1/");

	modelosExtra.push_back(new Model("models/terreno/scene.gltf"));
	posicionesModelosExtra.push_back(glm::vec3(0.0f, 0.0f, 0.0f));
	rotacionesModelosExtra.push_back(glm::vec3(0.0f)); // sin rotación
	escalasModelosExtra.push_back(glm::vec3(5.0f));

	modelosExtra.push_back(new Model("models/volcan2/scene.gltf"));
	posicionesModelosExtra.push_back(glm::vec3(100.0f, -3.0f, -50.0f));
	rotacionesModelosExtra.push_back(glm::vec3(0.0f, 0.0f, -90.0f)); // rotar sobre X
	escalasModelosExtra.push_back(glm::vec3(20.0f));

	modelosExtra.push_back(new Model("models/volcan3/scene.gltf"));
	posicionesModelosExtra.push_back(glm::vec3(-100.0f, 0.0f, -5.0f));
	rotacionesModelosExtra.push_back(glm::vec3(0.0f, 0.0f, -90.0f)); // rotar sobre X
	escalasModelosExtra.push_back(glm::vec3(20.0f));

	modelosExtra.push_back(new Model("models/volcan4/scene.gltf"));
	posicionesModelosExtra.push_back(glm::vec3(15.0f, 0.0f, 100.0f));
	rotacionesModelosExtra.push_back(glm::vec3(0.0f, 0.0f, -90.0f)); // sin rotación
	escalasModelosExtra.push_back(glm::vec3(20.0f));

	std::cout << "Modelos extra cargados: " << modelosExtra.size() << std::endl;
}

Volcan::~Volcan()
{
	if (model != nullptr)
		delete model;

	for (auto* m : modelosExtra)
		delete m;
	modelosExtra.clear();
	posicionesModelosExtra.clear();
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
		rutaModelo = "models/volcan4/scene.gltf";
		break;
	default:
		std::cerr << "Volcán no válido\n";
		return;
	}

	model = new Model(rutaModelo.c_str());
	std::cout << "Modelo principal cargado: " << rutaModelo << std::endl;
}

void Volcan::Dibujar(Shader& shader, Camera& camara)
{
	interfazSkybox.Dibujar(camara);

	if (model)
		model->Draw(shader, camara);

	// Dibujar modelos extra con sus posiciones
	for (size_t i = 0; i < modelosExtra.size(); ++i)
	{
		Model* extra = modelosExtra[i];
		glm::vec3 pos = posicionesModelosExtra[i];
		glm::vec3 rot = rotacionesModelosExtra[i];
		glm::vec3 escala = escalasModelosExtra[i];

		glm::mat4 rotX = glm::rotate(glm::mat4(1.0f), glm::radians(rot.x), glm::vec3(1, 0, 0));
		glm::mat4 rotY = glm::rotate(glm::mat4(1.0f), glm::radians(rot.y), glm::vec3(0, 1, 0));
		glm::mat4 rotZ = glm::rotate(glm::mat4(1.0f), glm::radians(rot.z), glm::vec3(0, 0, 1));
		glm::mat4 rotation = rotZ * rotY * rotX;

		glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), pos) * rotation * glm::scale(glm::mat4(1.0f), escala);

		auto& meshes = extra->GetMeshes();
		for (size_t j = 0; j < meshes.size(); ++j)
		{
			meshes[j].Draw(shader, camara, modelMatrix);
		}
	}



	static ShaderCollider shaderCollider;

	if (model)
		for (auto& m : model->GetMeshes())
			m.collider.Draw(shaderCollider, camara);

	for (auto* extra : modelosExtra)
		for (auto& m : extra->GetMeshes())
			m.collider.Draw(shaderCollider, camara);

	// Verificar colisiones entre el modelo principal y cada modelo extra
	if (model && !colisionDetectada)
	{
		const auto& meshes1 = model->GetMeshes();

		for (auto* extra : modelosExtra)
		{
			const auto& meshes2 = extra->GetMeshes();

			for (const auto& m1 : meshes1)
			{
				for (const auto& m2 : meshes2)
				{
					if (m1.collider.intersects(m2.collider))
					{
						std::cout << "⚠️ ¡Colisión detectada entre modelos!\n";
						colisionDetectada = true;
						break;
					}
				}
				if (colisionDetectada)
					break;
			}
			if (colisionDetectada)
				break;
		}
	}
}
