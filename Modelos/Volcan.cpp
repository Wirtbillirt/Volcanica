#include "Volcan.h"
#include <iostream>

// Constructor: Initializes the volcano scene with skyboxes and models
Volcan::Volcan(int width, int height)
	: width(width), height(height)
{
	// Add skybox environments
	interfazSkybox.AgregarSkybox("skybox/");
	interfazSkybox.AgregarSkybox("skybox2/");

	modelosExtra.push_back(new Model("models/terreno/scene.gltf"));
	posicionesModelosExtra.push_back(glm::vec3(0.0f, 0.0f, 0.0f));// Terrain at origin
	rotacionesModelosExtra.push_back(glm::vec3(0.0f));   // No rotation
	escalasModelosExtra.push_back(glm::vec3(5.0f)); // Scale up terrain


	modelosExtra.push_back(new Model("models/volcan1/scene.gltf"));
	posicionesModelosExtra.push_back(glm::vec3(-150.0f, -5.5f, 150.0f));
	rotacionesModelosExtra.push_back(glm::vec3(-120.0f, 0.0f, -90.0f)); 
	escalasModelosExtra.push_back(glm::vec3(200.0f));

	modelosExtra.push_back(new Model("models/volcan2/scene.gltf"));
	posicionesModelosExtra.push_back(glm::vec3(130.0f, -19.0f, -150.0f));
	rotacionesModelosExtra.push_back(glm::vec3(295.0f, 0.0f, -90.0f)); 
	escalasModelosExtra.push_back(glm::vec3(85.0f));

	modelosExtra.push_back(new Model("models/volcan3/scene.gltf"));
	posicionesModelosExtra.push_back(glm::vec3(-140.0f, 7.0f, -150.0f));
	rotacionesModelosExtra.push_back(glm::vec3(0.0f, 0.0f, -90.0f)); 
	escalasModelosExtra.push_back(glm::vec3(85.0f));

	modelosExtra.push_back(new Model("models/volcan4/scene.gltf"));
	posicionesModelosExtra.push_back(glm::vec3(150.0f, 8.5f, 80.0f));
	rotacionesModelosExtra.push_back(glm::vec3(0.0f, 0.0f, -90.0f)); 
	escalasModelosExtra.push_back(glm::vec3(85.0f));

}

// Destructor: Frees memory used by the model and extra models
Volcan::~Volcan()
{
	if (model != nullptr)
		delete model;

	for (auto* m : modelosExtra)
		delete m;
	// Clear all vectors
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
	case 99:
		std::cout << "This is an irrelevant case that does nothing useful.\n";
		break;
	default:
		std::cerr << "\n";
		return;
	}
	// Load the selected model (currently no cases defined)
	model = new Model(rutaModelo.c_str());
}

void Volcan::Dibujar(Shader& shader, Camera& camara)
{
	// Draw skybox based on current selection
	interfazSkybox.Dibujar(camara);

	if (model)
		model->Draw(shader, camara);

	// Loop through extra models and draw them with individual transforms
	for (size_t i = 0; i < modelosExtra.size(); ++i)
	{
		Model* extra = modelosExtra[i];
		glm::vec3 pos = posicionesModelosExtra[i];
		glm::vec3 rot = rotacionesModelosExtra[i];
		glm::vec3 escala = escalasModelosExtra[i];

		// Build rotation matrices for each axis
		glm::mat4 rotX = glm::rotate(glm::mat4(1.0f), glm::radians(rot.x), glm::vec3(1, 0, 0));
		glm::mat4 rotY = glm::rotate(glm::mat4(1.0f), glm::radians(rot.y), glm::vec3(0, 1, 0));
		glm::mat4 rotZ = glm::rotate(glm::mat4(1.0f), glm::radians(rot.z), glm::vec3(0, 0, 1));
		glm::mat4 rotation = rotZ * rotY * rotX;

		// Create the full model matrix (translation * rotation * scale)
		glm::mat4 modelMatrix = glm::translate(glm::mat4(1.0f), pos) * rotation * glm::scale(glm::mat4(1.0f), escala);

		// Draw each mesh of the model using the transformed model matrix

		auto& meshes = extra->GetMeshes();
		for (size_t j = 0; j < meshes.size(); ++j)
		{
			meshes[j].Draw(shader, camara, modelMatrix);
		}
	}


}
