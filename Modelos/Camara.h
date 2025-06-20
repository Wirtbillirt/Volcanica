#ifndef CAMERA_CLASS_H
#define CAMERA_CLASS_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/vector_angle.hpp>
#include "imgui.h"
#include "shaderClass.h"

class Camera
{
public:
	// Vectores principales
	glm::vec3 Position;
	glm::vec3 Orientation = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 Up = glm::vec3(0.0f, 1.0f, 0.0f);
	glm::mat4 cameraMatrix = glm::mat4(1.0f);
	glm::vec3 posicionInicial;


	// Control del click inicial
	bool firstClick = true;

	// Dimensiones de la ventana
	int width;
	int height;

	// Velocidad y sensibilidad
	float speed = 0.1f;
	float sensitivity = 100.0f;

	// Constructor
	Camera(int width, int height, glm::vec3 position);

	// Alternar modo de movimiento
	void AlternarRestriccionMovimiento();
	bool EsModoRestringido() const { return movimientoRestringido; }

	// Métodos públicos
	void updateMatrix(float FOVdeg, float nearPlane, float farPlane);
	void Matrix(Shader& shader, const char* uniform);
	void Inputs(GLFWwindow* window);

private:
	bool movimientoRestringido = false;
};

#endif
