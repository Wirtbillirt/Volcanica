#version 330 core
out vec4 FragColor;

in vec2 TexCoords;
in vec3 Normal;
in vec3 FragPos;
in vec3 Tangent;

struct Material {
    sampler2D texture_diffuse;
    sampler2D texture_normal;
    sampler2D texture_height;
};

uniform Material material;
uniform vec3 viewPos;

void main() {
    // Textura difusa
    vec4 diffuseColor = texture(material.texture_diffuse, TexCoords);

    // Si no hay textura, usa color rojo para debug
    if (diffuseColor.a < 0.1) {
        FragColor = vec4(1.0, 0.0, 0.0, 1.0);
    } else {
        FragColor = diffuseColor;
    }
}