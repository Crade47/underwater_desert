#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec4 aColor;
layout (location = 2) in vec2 aTexCoor;

uniform mat4 model; 
uniform mat4 view;
uniform mat4 projection;

out vec4 vertexColor;
out vec2 TexCoor;

void main() {

    gl_Position = projection * view * model * vec4(aPos.x, aPos.y, aPos.z, 1.0);
    vertexColor = aColor;
    TexCoor = aTexCoor;
}