#version 330 core


in vec4 vertexColor;
in vec2 TexCoords;

struct Material {
    sampler2D texture_diffuse1;
    sampler2D texture_diffuse2;

};

uniform Material material;


out vec4 FragColor;

void main() {
    vec4 texColor1 = texture(material.texture_diffuse1, TexCoords);
    vec4 texColor2 = texture(material.texture_diffuse2, TexCoords);
    FragColor = mix(texColor1, texColor2, 0.1);
}