#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shaderClass.h"
#include "Camara.h"

#include "scenes.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

#include "AudioManager.h"

const unsigned int width = 800;
const unsigned int height = 800;

float skyboxVertices[] =
{
	//   Coordinates
	-1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f,  1.0f,
	 1.0f, -1.0f, -1.0f,
	-1.0f, -1.0f, -1.0f,
	-1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f,  1.0f,
	 1.0f,  1.0f, -1.0f,
	-1.0f,  1.0f, -1.0f
};

unsigned int skyboxIndices[] =
{
	// Right
	1, 2, 6,
	6, 5, 1,
	// Left
	0, 4, 7,
	7, 3, 0,
	// Top
	4, 5, 6,
	6, 7, 4,
	// Bottom
	0, 3, 2,
	2, 1, 0,
	// Back
	0, 1, 5,
	5, 4, 0,
	// Front
	3, 7, 6,
	6, 2, 3
};

enum class Escena
{
	PantallaInicio,
	SeleccionVolcan,
	VistaVolcan
	// Puedes agregar más como Resultado, CargaModelo, etc.
};

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "Pantalla de Inicio", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);
	gladLoadGL();
	glViewport(0, 0, width, height);

	// Inicializar ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();
	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	Shader shaderProgram("default.vert", "default.frag");

	glm::vec4 lightColor = glm::vec4(1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::translate(glm::mat4(1.0f), lightPos);

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	glEnable(GL_DEPTH_TEST);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 10.0f));

	// Crear la pantalla de inicio
	PantallaInicio pantallaInicio(width, height);
	SeleccionVolcan seleccionVolcan(width, height);
	Volcan volcanScene(width, height);
	Escena escenaActual = Escena::PantallaInicio;

	// Administrar sonido
	AudioManager audio;

	audio.playBackgroundMusic("assets/music/ambient.wav");
	audio.setBackgroundVolume(0.5f);


	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		// Iniciar frame ImGui
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		switch (escenaActual)
		{
		case Escena::PantallaInicio:
			pantallaInicio.Dibujar();
			if (pantallaInicio.iniciarPresionado)
				escenaActual = Escena::SeleccionVolcan;
			break;

		case Escena::SeleccionVolcan:
			seleccionVolcan.Dibujar();
			if (seleccionVolcan.volcanSeleccionado != 0)
			{
				volcanScene.CargarModelo(seleccionVolcan.volcanSeleccionado);
				escenaActual = Escena::VistaVolcan;
			}
			break;

		case Escena::VistaVolcan:
			if (!ImGui::GetIO().WantCaptureMouse)
				camera.Inputs(window);

			camera.updateMatrix(45.0f, 0.1f, 10000.0f);
			volcanScene.Dibujar(shaderProgram, camera);

			// Botón "Regresar" en esquina superior izquierda
			ImGui::SetNextWindowPos(ImVec2(10, 10));
			ImGui::Begin("Regresar", nullptr,
				ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_AlwaysAutoResize |
				ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoSavedSettings);

			if (ImGui::Button("Regresar"))
			{
				// Cambia de vuelta a la escena de selección
				escenaActual = Escena::SeleccionVolcan;
				seleccionVolcan.volcanSeleccionado = 0;
			}

			ImGui::End();
			break;
		}


		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}



	// Limpieza
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	shaderProgram.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;

}
