cmake_minimum_required(VERSION 3.20)
project(volcania)

set(CMAKE_CXX_STANDARD 20)

# Dependencias
find_package(glfw3 REQUIRED)
find_package(assimp REQUIRED)
find_package(glm REQUIRED)

# Configurar rutas de inclusión
include_directories(
        ${GLFW_INCLUDE_DIRS}
        ${assimp_INCLUDE_DIRS}
        ${glm_INCLUDE_DIRS}
        "${PROJECT_SOURCE_DIR}/Header Files"
        "${PROJECT_SOURCE_DIR}/Source file"
        "${PROJECT_SOURCE_DIR}/Source file/glad/include"
)

# Lista explícita de archivos fuente (evitar GLOB)
set(SOURCE_FILES
        "${PROJECT_SOURCE_DIR}/Source file/main.cpp"
        "${PROJECT_SOURCE_DIR}/Source file/Shader.cpp"
        "${PROJECT_SOURCE_DIR}/Source file/Model.cpp"
        "${PROJECT_SOURCE_DIR}/Source file/Mesh.cpp"
        "${PROJECT_SOURCE_DIR}/Source file/VAO.cpp"
        "${PROJECT_SOURCE_DIR}/Source file/VBO.cpp"
        "${PROJECT_SOURCE_DIR}/Source file/EBO.cpp"
        "${PROJECT_SOURCE_DIR}/Source file/glad/src/glad.c"
)

add_executable(${PROJECT_NAME}
        ${SOURCE_FILES}
)

# Enlazar bibliotecas
target_link_libraries(${PROJECT_NAME}
        glfw
        assimp::assimp
        opengl32
        glm::glm
)

# Copiar recursos al ejecutable
add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/models"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/models"
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_directory
        "${PROJECT_SOURCE_DIR}/shaders"
        "$<TARGET_FILE_DIR:${PROJECT_NAME}>/shaders"
)
