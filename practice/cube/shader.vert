#version 330 core

layout (location = 0) in vec4 Pos;
out vec4 Color;
uniform mat4 Transform;

vec4 colors[3] = vec4[3](
    vec4(1, 0, 0, 1),
    vec4(0, 1, 0, 1),
    vec4(0, 0, 1, 1)
);

void main() {
    gl_Position = Transform * Pos;
    Color = colors[gl_VertexID % 3];
}