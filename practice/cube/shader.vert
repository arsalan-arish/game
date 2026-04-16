#version 330 core

layout (location = 0) in vec4 Pos;
layout (location = 1) in vec4 Color;

out vec4 inColor;

void main() {
    gl_Position = Pos;
    inColor = Color;
}
