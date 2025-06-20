#include "Skybox.h"
#include <stb/stb_image.h>
#include <iostream>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

float skyboxVertices[] = {
    -1.0f, -1.0f,  1.0f,  1.0f, -1.0f,  1.0f,
     1.0f, -1.0f, -1.0f, -1.0f, -1.0f, -1.0f,
    -1.0f,  1.0f,  1.0f,  1.0f,  1.0f,  1.0f,
     1.0f,  1.0f, -1.0f, -1.0f,  1.0f, -1.0f
};

unsigned int skyboxIndices[] = {
    1, 2, 6, 6, 5, 1, // derecha
    0, 4, 7, 7, 3, 0, // izquierda
    4, 5, 6, 6, 7, 4, // arriba
    0, 3, 2, 2, 1, 0, // abajo
    0, 1, 5, 5, 4, 0, // atrás
    3, 7, 6, 6, 2, 3  // frente
};

Skybox::Skybox()
    : skyboxShader("skybox.vert", "skybox.frag")
{
    // Inicializar VAO y buffers
    glGenVertexArrays(1, &skyboxVAO);
    glGenBuffers(1, &skyboxVBO);
    glGenBuffers(1, &skyboxEBO);

    glBindVertexArray(skyboxVAO);

    glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVertices), skyboxVertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndices), skyboxIndices, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // Configurar shader
    skyboxShader.Activate();
    glUniform1i(glGetUniformLocation(skyboxShader.ID, "skybox"), 0);
}

void Skybox::CargarSkybox(const std::string& directory)
{
    std::string faces[6] = {
        directory + "right.jpg",
        directory + "left.jpg",
        directory + "top.jpg",
        directory + "bottom.jpg",
        directory + "front.jpg",
        directory + "back.jpg"
    };

    GLuint textureID;
    glGenTextures(1, &textureID);
    glBindTexture(GL_TEXTURE_CUBE_MAP, textureID);

    for (unsigned int i = 0; i < 6; ++i)
    {
        int width, height, nrChannels;
        stbi_set_flip_vertically_on_load(false);
        unsigned char* data = stbi_load(faces[i].c_str(), &width, &height, &nrChannels, 0);
        if (data)
        {
            glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB,
                width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
            stbi_image_free(data);
        }
        else
        {
            std::cout << "Error cargando: " << faces[i] << std::endl;
        }
    }

    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
    glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);

    cubemapTextures.push_back(textureID);
}

void Skybox::CambiarSkybox(int index)
{
    if (index >= 0 && index < cubemapTextures.size())
        skyboxActual = index;
}

void Skybox::Dibujar(Camera& camara)
{
    if (cubemapTextures.empty())
        return;

    glDepthFunc(GL_LEQUAL);
    skyboxShader.Activate();

    glm::mat4 view = glm::mat4(glm::mat3(glm::lookAt(camara.Position, camara.Position + camara.Orientation, camara.Up)));
    glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)camara.width / camara.height, 0.1f, 100.0f);

    glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "view"), 1, GL_FALSE, glm::value_ptr(view));
    glUniformMatrix4fv(glGetUniformLocation(skyboxShader.ID, "projection"), 1, GL_FALSE, glm::value_ptr(projection));

    glBindVertexArray(skyboxVAO);
    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTextures[skyboxActual]);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
    glDepthFunc(GL_LESS);
}

Skybox::~Skybox()
{
    glDeleteVertexArrays(1, &skyboxVAO);
    glDeleteBuffers(1, &skyboxVBO);
    glDeleteBuffers(1, &skyboxEBO);

    for (GLuint texture : cubemapTextures)
        glDeleteTextures(1, &texture);

    skyboxShader.Delete();
}
