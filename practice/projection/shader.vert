#version 330 core

layout (location = 0) in vec3 Pos;
uniform mat4 Transform;

void main() {
    gl_Position = Transform * vec4(Pos, 1.0f);
}