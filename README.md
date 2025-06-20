# 🌋 Volcánica

## Overview

This project presents the development of an educational virtual tour in a three-dimensional environment using OpenGL as the foundational platform for real-time graphical representation. The proposal integrates multiple advanced concepts in interactive visualization and digital simulation, applied to the exploration of volcanic formations in an educational context.

The system architecture enables dynamic loading of models and terrains through structured files that specify paths, scales, and visual attributes, providing flexible resource management. Navigation is achieved through a freely movable camera along all three spatial axes, allowing users to explore the environment from different angles and heights. The visual orientation continuously adapts to the observer’s position and direction, ensuring an immersive experience.

Additionally, an enveloping background environment can be dynamically modified during execution, enabling simulation of different environmental scenarios. The system also incorporates positional ambient sound synchronized with the user's location to reinforce the sensory coherence of the experience. Finally, a minimalist graphical interface allows intuitive adjustment of navigation parameters and visual settings.

The system’s runtime loop includes resource loading, perspective updating, scene rendering, ambient sound playback, and user interface visualization—all in real time.

---

## 🔑 Keywords

Virtual tour, 3D visualization, interactive simulation, educational environment, OpenGL, real-time navigation, three-dimensional models, positional sound, graphical interface, graphical representation.

---

## ▶️ How to Run the Project

This is an OpenGL project developed using **Visual Studio 2022** on Windows. Follow these steps to run it successfully:

### Prerequisites

- **Visual Studio 2022** with:
  - “Desktop Development with C++” workload
- **OpenGL-capable hardware** (Intel UHD Graphics or better)
- **Windows 10 Pro 64-bit**
- Properly installed and configured libraries:
  - **GLFW**
  - **GLAD**
  - **GLM**
  - **stb_image**
  

---

### Steps to Execute

1. **Clone or download the repository**:

   ```bash
   git clone https://github.com/Wirtbillirt/Volcanica.git
   ```

   Or download the ZIP from GitHub and extract it locally.

2. **Open the project**:
   - Double-click on the `Volcanica.sln` file to open it in Visual Studio 2022.

3. **Set up Include and Library Directories**:
   - Go to `Project > Properties > VC++ Directories`.
   - Add the paths to:
     - Include Directories: `GLFW/include`, `GLAD/include`, `GLM`, etc.
     - Library Directories: where `glfw3.lib` and other `.lib` files are located.

4. **Link required libraries**:
   - Under `Linker > Input > Additional Dependencies`, add:
     ```
     opengl32.lib
     glfw3.lib
     ```

5. **Verify glad.c is included** in the source files:
   - Include `glad.c` in the project (add it to the source folder if missing).
   - In your main file, include:

     ```cpp
     #include <glad/glad.h>
     #include <GLFW/glfw3.h>
     ```

6. **Initialize GLAD after creating the context**:

   ```cpp
   if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
       std::cerr << "Failed to initialize GLAD" << std::endl;
       return -1;
   }
   ```

7. **Build the solution**:
   - Press `Ctrl + Shift + B` or go to `Build > Build Solution`.
   - Make sure there are no build errors.

8. **Run the application**:
   - Press `F5` or click "Start Debugging".
   - The window should open showing the 3D volcanic environment.

9. **Controls and features**:
   - **W/A/S/D** keys for movement
   - **Mouse** for camera look-around
   - **ImGui UI** for dynamic settings like background/environment changes
   - **Positional audio** based on user movement
   - **Real-time rendering** with immersive transitions

---

> 📺 **Watch the demo video here:**  
> [(https://youtu.be/4nacvnic8so)]

---

## 👤 Authors

- **Br. Wilberth Alejandro Pérez Loredo**  
- **Br. Oscar Moises Perez Delgado**
- **Br. Ángel Rafael Maltez Navarrete**  
- **Br. Brandon Brandy Downs Hebberth**  

---

## 🧪 Testing Information

This project has been successfully tested on systems with the following minimum specifications:

- **Processor**: Intel Core i5-8350U (8th Generation)  
- **Memory**: 8 GB RAM  
- **Storage**: 5 GB 
- **Display**: 14-inch screen with FHD resolution (1366 x 768)  
- **Graphics**: Intel UHD Integrated Graphics  
- **Operating System**: Windows 10 Pro 64-bit  

For best performance, systems with dedicated GPUs (NVIDIA or AMD) are recommended.

---

## 📄 License

## 📄 License
[![License: MIT](https://img.shields.io/badge/License-MIT-yellow.svg)](LICENSE)

Distributed under the MIT License. See [LICENSE](LICENSE) for details.
