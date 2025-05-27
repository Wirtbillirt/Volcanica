#include <iostream>
#include <glad/glad.h>
#include <GLFW/glfw3.h>

// ImGui
#include <imgui.h>
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "PantallaInicio.h"
#include "SeleccionVolcan.h"

const unsigned int width = 800;
const unsigned int height = 800;

PantallaInicio pantallaInicio(width, height);	
SeleccionVolcan seleccionVolcan(width, height);

enum class Escena
{
	PantallaInicio,
	SeleccionVolcan,
	Resultado
};

Escena escenaActual = Escena::PantallaInicio;

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "Solo ImGui", NULL, NULL);
	if (!window)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGL()) {
		std::cerr << "Failed to initialize GLAD\n";
		return -1;
	}
	glViewport(0, 0, width, height);

	// Inicializar ImGui
	IMGUI_CHECKVERSION();
	ImGui::CreateContext();
	ImGuiIO& io = ImGui::GetIO(); (void)io;
	ImGui::StyleColorsDark();

	ImGui_ImplGlfw_InitForOpenGL(window, true);
	ImGui_ImplOpenGL3_Init("#version 330");

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	// Bucle principal
	while (!glfwWindowShouldClose(window))
	{
		glfwPollEvents();
		glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		if (!pantallaInicio.iniciarPresionado)
		{
			pantallaInicio.Dibujar();
		}
		else if (seleccionVolcan.volcanSeleccionado == 0)
		{
			seleccionVolcan.Dibujar();
		}
		else
		{
			ImGui::Begin("Resultado");
			ImGui::Text("Has seleccionado Volcán %d", seleccionVolcan.volcanSeleccionado);
			ImGui::End();
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
		glfwSwapBuffers(window);
	}

	// Limpieza
	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
