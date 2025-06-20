#include "scenes.h"
#include "AudioManager.h"
#include "CreditosScene.h"

#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>

const unsigned int width = 1366;
const unsigned int height = 768;

enum class Escena {
	SeleccionVolcan,
	VistaVolcan,
	Creditos
};

int main()
{
	glfwInit();
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	GLFWwindow* window = glfwCreateWindow(width, height, "Volcanica", NULL, NULL);
	if (window == NULL)
	{
		std::cout << "Failed to create GLFW window" << std::endl;
		glfwTerminate();
		return -1;
	}
	glfwMakeContextCurrent(window);

	if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
		std::cout << "Failed to initialize GLAD" << std::endl;
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

	Shader shaderProgram("default.vert", "default.frag");

	glm::vec4 lightColor = glm::vec4(1.0f);
	glm::vec3 lightPos = glm::vec3(0.5f, 0.5f, 0.5f);
	glm::mat4 lightModel = glm::translate(glm::mat4(1.0f), lightPos);

	shaderProgram.Activate();
	glUniform4f(glGetUniformLocation(shaderProgram.ID, "lightColor"), lightColor.x, lightColor.y, lightColor.z, lightColor.w);
	glUniform3f(glGetUniformLocation(shaderProgram.ID, "lightPos"), lightPos.x, lightPos.y, lightPos.z);

	glEnable(GL_DEPTH_TEST);

	Camera camera(width, height, glm::vec3(0.0f, 0.0f, 10.0f));

	SeleccionVolcan seleccionVolcan(width, height);
	Volcan volcanScene(width, height);
	CreditosScene creditosScene(width, height);
	Escena escenaActual = Escena::SeleccionVolcan;

	AudioManager audio;
	audio.playBackgroundMusic("assets/music/ambient.wav");
	audio.setBackgroundVolume(0.5f);

	while (!glfwWindowShouldClose(window))
	{
		glClearColor(0.07f, 0.13f, 0.17f, 1.0f);
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();

		switch (escenaActual)
		{
		case Escena::SeleccionVolcan:
			seleccionVolcan.Dibujar();
			if (seleccionVolcan.volcanSeleccionado != 0)
			{
				audio.setEffectVolume(0.5f);
				audio.playSoundEffect("assets/effect/click.wav");

				if (seleccionVolcan.volcanSeleccionado == 2)
				{
					escenaActual = Escena::Creditos;
				}
				else if (seleccionVolcan.volcanSeleccionado == 3)
				{
					glfwSetWindowShouldClose(window, GLFW_TRUE);
				}
				else
				{
					volcanScene.CargarModelo(seleccionVolcan.volcanSeleccionado);
					escenaActual = Escena::VistaVolcan;
				}
			}
			break;

		case Escena::VistaVolcan:
		{
			if (!ImGui::GetIO().WantCaptureMouse)
			{
				static bool teclaPresionada = false;
				if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS && !teclaPresionada)
				{
					camera.AlternarRestriccionMovimiento();
					teclaPresionada = true;
				}
				if (glfwGetKey(window, GLFW_KEY_R) == GLFW_RELEASE)
				{
					teclaPresionada = false;
				}

				camera.Inputs(window);
			}

			float fov = camera.EsModoRestringido() ? 45.0f : 60.0f;
			camera.updateMatrix(fov, 0.1f, 10000.0f);
			volcanScene.Dibujar(shaderProgram, camera);

			ImGui::SetNextWindowPos(ImVec2(10, 10));
			ImGui::Begin("Regresar", nullptr,
				ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_AlwaysAutoResize |
				ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoSavedSettings);

			if (ImGui::Button("Regresar"))
			{
				audio.setEffectVolume(0.5f);
				audio.playSoundEffect("assets/effect/click.wav");

				escenaActual = Escena::SeleccionVolcan;
				seleccionVolcan.volcanSeleccionado = 0;
			}
			ImGui::End();
			ImGui::SetNextWindowPos(ImVec2(10, 100)); 
			ImGui::Begin("ModoCamara", nullptr,
				ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_AlwaysAutoResize |
				ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoSavedSettings |
				ImGuiWindowFlags_NoBackground |
				ImGuiWindowFlags_NoInputs);

			ImGui::Text(camera.EsModoRestringido() ? "Modo caminata" : "Modo libre");

			ImGui::SetNextWindowPos(ImVec2(10, 140));
			ImGui::Begin("AyudaCamara", nullptr,
				ImGuiWindowFlags_NoTitleBar |
				ImGuiWindowFlags_NoResize |
				ImGuiWindowFlags_AlwaysAutoResize |
				ImGuiWindowFlags_NoMove |
				ImGuiWindowFlags_NoSavedSettings |
				ImGuiWindowFlags_NoBackground |
				ImGuiWindowFlags_NoInputs);

			ImGui::Text("Presiona R para cambiar de modo");
			ImGui::End();
			ImGui::End();
			break;
		}
			case Escena::Creditos:
			creditosScene.Dibujar();
				if (creditosScene.regresarPresionado)
				{
				creditosScene.regresarPresionado = false;
				escenaActual = Escena::SeleccionVolcan;
				seleccionVolcan.volcanSeleccionado = 0;
				}
			break;
		break;
		}

		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	ImGui_ImplOpenGL3_Shutdown();
	ImGui_ImplGlfw_Shutdown();
	ImGui::DestroyContext();

	shaderProgram.Delete();
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
