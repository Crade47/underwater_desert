#version 330 core


in vec4 vertexColor;
in vec2 TexCoor;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;

};

uniform Material material;


out vec4 FragColor;

void main() {
    FragColor = mix(texture(material.texture_diffuse1, TexCoor), texture(material.texture_diffuse2, TexCoor), 0.1);
}