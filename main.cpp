#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "Volcano.h"
#include "Crater.h"
#include <iostream>
#include <stb_image.h>

const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// Variables de cámara
glm::vec3 cameraPos = glm::vec3(0.0f, 2.5f, 6.0f);
glm::vec3 cameraFront = glm::vec3(0.0f, -0.2f, -1.0f);
glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);

// Variables de control
float yaw = -90.0f;
float pitch = 0.0f;
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;
bool cursorEnabled = false;
float deltaTime = 0.0f;
float lastFrame = 0.0f;
float rotationAngle = 0.0f;

// Prototipos de funciones
void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void processInput(GLFWwindow* window);
unsigned int compileShader(GLenum type, const char* source);
bool CheckVolcanoCollision(const glm::vec3& position, const glm::mat4& model);
bool CheckCraterCollision(const glm::vec3& position, const glm::mat4& model);
glm::vec3 HandleCollision(const glm::vec3& newPos, const glm::vec3& oldPos);

const char* vertexShaderSource = R"(
    #version 330 core
    layout (location = 0) in vec3 aPos;
    layout (location = 1) in vec2 aTexCoord;
    out vec2 TexCoord;
    uniform mat4 model;
    uniform mat4 view;
    uniform mat4 projection;
    void main() {
        gl_Position = projection * view * model * vec4(aPos, 1.0);
        TexCoord = aTexCoord;
    }
)";

const char* fragmentShaderSource = R"(
    #version 330 core
    in vec2 TexCoord;
    out vec4 FragColor;
    uniform sampler2D texture1;
    void main() {
        FragColor = texture(texture1, TexCoord);
    }
)";

int main() {  
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Volcan", NULL, NULL);
    if (!window) {
        std::cerr << "Error creando ventana GLFW" << std::endl;
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);

    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    // Compilar shaders
    unsigned int vertexShader = compileShader(GL_VERTEX_SHADER, vertexShaderSource);
    unsigned int fragmentShader = compileShader(GL_FRAGMENT_SHADER, fragmentShaderSource);

    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        std::cerr << "Error de enlace de shaders: " << infoLog << std::endl;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Crear objetos
    Volcano volcano("volcan.png", 1.0f, 0.4f, 1.5f, 40);
    Crater crater("crater.png", 0.45f, 0.2f, 40);

    glEnable(GL_DEPTH_TEST);
    glUseProgram(shaderProgram);
    glUniform1i(glGetUniformLocation(shaderProgram, "texture1"), 0);

    while (!glfwWindowShouldClose(window)) {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;
        rotationAngle += deltaTime * 30.0f;

        processInput(window);

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glm::mat4 view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);
        glm::mat4 projection = glm::perspective(glm::radians(45.0f),
            (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "view"),
            1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shaderProgram, "projection"),
            1, GL_FALSE, glm::value_ptr(projection));

        // Dibujar volcán
        glm::mat4 volcanoModel = glm::rotate(glm::mat4(1.0f),
            glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));
        volcano.Draw(shaderProgram, volcanoModel);

        // Dibujar cráter
        glm::mat4 craterModel = glm::translate(volcanoModel, glm::vec3(0.0f, 1.5f, 0.0f));
        craterModel = glm::scale(craterModel, glm::vec3(0.9f));
        craterModel = glm::rotate(craterModel,
            glm::radians(-rotationAngle * 0.8f), glm::vec3(0.0f, 1.0f, 0.0f));
        crater.Draw(shaderProgram, craterModel);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwTerminate();
    return 0;
}

// Implementación de funciones de colisión
bool CheckVolcanoCollision(const glm::vec3& position, const glm::mat4& model) {
    glm::vec3 localPos = glm::vec3(glm::inverse(model) * glm::vec4(position, 1.0f));

    float height = 1.5f;
    float baseRadius = 1.0f;
    float topRadius = 0.4f;

    if (localPos.y < 0 || localPos.y > height) return false;

    float allowedRadius = baseRadius - (baseRadius - topRadius) * (localPos.y / height);
    float distance = sqrt(localPos.x * localPos.x + localPos.z * localPos.z);

    return distance <= allowedRadius;
}

bool CheckCraterCollision(const glm::vec3& position, const glm::mat4& model) {
    glm::vec3 localPos = glm::vec3(glm::inverse(model) * glm::vec4(position, 1.0f));

    float radius = 0.35f;
    float depth = 0.2f;

    if (localPos.y < -depth || localPos.y > 0) return false;

    float distance = sqrt(localPos.x * localPos.x + localPos.z * localPos.z);
    return distance <= radius;
}

glm::vec3 HandleCollision(const glm::vec3& newPos, const glm::vec3& oldPos) {
    glm::mat4 volcanoModel = glm::rotate(glm::mat4(1.0f),
        glm::radians(rotationAngle), glm::vec3(0.0f, 1.0f, 0.0f));

    glm::mat4 craterModel = glm::translate(volcanoModel, glm::vec3(0.0f, 1.5f, 0.0f));
    craterModel = glm::scale(craterModel, glm::vec3(0.9f));
    craterModel = glm::rotate(craterModel,
        glm::radians(-rotationAngle * 0.8f), glm::vec3(0.0f, 1.0f, 0.0f));

    bool volcanoCollision = CheckVolcanoCollision(newPos, volcanoModel);
    bool craterCollision = CheckCraterCollision(newPos, craterModel);

    if (!volcanoCollision && !craterCollision) return newPos;

    glm::vec3 moveDir = newPos - oldPos;
    glm::vec3 adjustedPos = oldPos;
    const float step = 0.1f;

    for (float t = 0.0f; t <= 1.0f; t += step) {
        glm::vec3 testPos = oldPos + moveDir * t;
        if (!CheckVolcanoCollision(testPos, volcanoModel) &&
            !CheckCraterCollision(testPos, craterModel)) {
            adjustedPos = testPos;
        }
    }

    return adjustedPos;
}

void processInput(GLFWwindow* window) {
    static glm::vec3 lastValidPos = cameraPos;

    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);

    // Toggle cursor
    static bool tabPressed = false;
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_PRESS && !tabPressed) {
        cursorEnabled = !cursorEnabled;
        glfwSetInputMode(window, GLFW_CURSOR,
            cursorEnabled ? GLFW_CURSOR_NORMAL : GLFW_CURSOR_DISABLED);
        firstMouse = true;
        tabPressed = true;
    }
    if (glfwGetKey(window, GLFW_KEY_TAB) == GLFW_RELEASE) tabPressed = false;

    if (!cursorEnabled) {
        float cameraSpeed = 2.5f * deltaTime;
        glm::vec3 newPos = cameraPos;

        if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS)
            newPos += cameraFront * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS)
            newPos -= cameraFront * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
            newPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS)
            newPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;

        cameraPos = HandleCollision(newPos, lastValidPos);
        lastValidPos = cameraPos;
    }
}

unsigned int compileShader(GLenum type, const char* source) {
    unsigned int shader = glCreateShader(type);
    glShaderSource(shader, 1, &source, NULL);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        std::cerr << "Error de compilación de shader: " << infoLog << std::endl;
    }
    return shader;
}

void framebuffer_size_callback(GLFWwindow* window, int width, int height) {
    glViewport(0, 0, width, height);
}

void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
    if (!cursorEnabled) {
        if (firstMouse) {
            lastX = xpos;
            lastY = ypos;
            firstMouse = false;
        }

        float xoffset = xpos - lastX;
        float yoffset = lastY - ypos;
        lastX = xpos;
        lastY = ypos;

        float sensitivity = 0.1f;
        xoffset *= sensitivity;
        yoffset *= sensitivity;

        yaw += xoffset;
        pitch += yoffset;

        if(pitch > 89.0f) pitch = 89.0f;
        if(pitch < -89.0f) pitch = -89.0f;

        glm::vec3 front;
        front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
        front.y = sin(glm::radians(pitch));
        front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
        cameraFront = glm::normalize(front);
    }
}