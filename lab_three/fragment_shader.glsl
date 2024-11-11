#version 330 core


in vec4 vertexColor;
in vec2 TexCoor;

uniform sampler2D texture_one;
uniform sampler2D texture_two;

out vec4 FragColor;

void main() {
    FragColor = mix(texture(texture_one, TexCoor), texture(texture_two, TexCoor), 0.6);
}