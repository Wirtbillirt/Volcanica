cmake_minimum_required(VERSION 3.20)
project(volcania)

set(CMAKE_CXX_STANDARD 20)

# Configurar GLFW
find_package(PkgConfig REQUIRED)
pkg_check_modules(GLFW REQUIRED glfw3)

# Incluir directorios de headers
include_directories(
        ${GLFW_INCLUDE_DIRS}
        "${PROJECT_SOURCE_DIR}/Header Files"            # Para VAO.h, EBO.h, Shader.h
        "${PROJECT_SOURCE_DIR}/Source file"             # Para stb_image.h (si está aquí)
        "${PROJECT_SOURCE_DIR}/Source file/glad/include"
        "cmake-build-debug/Resource Files/Shaders"
)

# Agregar TODOS los archivos fuente (CORREGIDO)
add_executable(${PROJECT_NAME}
        main.cpp
        "${PROJECT_SOURCE_DIR}/Source file/glad/src/glad.c"
        "${PROJECT_SOURCE_DIR}/Source file/EBO.cpp"
        "${PROJECT_SOURCE_DIR}/Source file/VAO.cpp"
        "${PROJECT_SOURCE_DIR}/Source file/VBO.cpp"
        "${PROJECT_SOURCE_DIR}/Source file/Shader.cpp"
        "Source file/stb_image.cpp"  # ¡Agrega Shader.cpp aquí!
)

# Enlazar bibliotecas
target_link_libraries(${PROJECT_NAME}
        ${GLFW_LIBRARIES}
        opengl32
)