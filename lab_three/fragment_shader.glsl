#version 330 core


in vec4 vertexColor;
in vec2 TexCoor;

uniform sampler2D texture_in;

out vec4 FragColor;

void main() {
    FragColor = texture(texture_in, TexCoor);
}