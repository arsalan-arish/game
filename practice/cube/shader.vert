#version 330 core

layout (location = 0) in vec4 Pos;
out vec4 Color;

vec4 colors[3] = vec4[3](
    vec4(1, 0, 0, 1),
    vec4(0, 1, 0, 1),
    vec4(0, 0, 1, 1)
);

void main() {
    gl_Position = Pos;
    Color = colors[gl_VertexID % 3];
}