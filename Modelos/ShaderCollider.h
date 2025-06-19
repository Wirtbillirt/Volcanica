#pragma once
#include "shaderClass.h"

class ShaderCollider : public Shader {
public:
    ShaderCollider()
        : Shader("collider.vert", "collider.frag")
        {
    }  // Usa tus shaders para colisiones
};
