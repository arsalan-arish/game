#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <cstdio>
#include <types.h>
#include <string>
#include <fstream>
#include <sstream>

#define BLACK 0.0f, 0.0f, 0.0f, 1.0f
#define RED 1.0f,  0.0f, 0.0f, 1.0f
#define GREEN 0.0f, 1.0f, 0.0f, 1.0f
#define BLUE 0.0f, 0.0f, 1.0f, 1.0f


std::string readFile(const char* path) {
    std::ifstream file(path);
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

GLFWwindow* init(u16 width, u16 height, const char* title) {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    glfwMakeContextCurrent(window);
    if (!gladLoadGL()) {
        printf("Failed to load GL\n");
        exit(1);
    }
    return window;
}

void checkForErrors(u32 vertexShader, u32 fragmentShader, u32 shaderProgram) {

    int success;
    char infoLog[512];

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("VERTEX_SHADER::COMPILATION_FAILED\n%s", infoLog);
    } else {
        printf("VERTEX_SHADER --> Compiled successfully\n");
    }

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(fragmentShader, 512, NULL, infoLog);
        printf("FRAGMENT_SHADER::COMPILATION_FAILED\n%s", infoLog);
    } else {
        printf("FRAGMENT_SHADER --> Compiled successfully\n");
    }

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("SHADER_PROGRAM::LINKING_FAILED\n%s", infoLog);
    } else {
        printf("SHADER_PROGRAM --> Compiled successfully\n");
    }
}

u32 createShaderProgram() {

    auto vertSourceStr = readFile("practice/cube/shader.vert");
    auto fragSourceStr = readFile("practice/cube/shader.frag");
    
    const char* vertSources[] = {vertSourceStr.c_str()};
    const char* fragSources[] = {fragSourceStr.c_str()};

    u32 vertArraySize = sizeof(vertSources) / sizeof(vertSources[0]);
    u32 fragArraySize = sizeof(fragSources) / sizeof(fragSources[0]);

    u32 vertShader = glCreateShader(GL_VERTEX_SHADER);
    u32 fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertShader, vertArraySize, vertSources, NULL);
    glShaderSource(fragShader, fragArraySize, fragSources, NULL);
    glCompileShader(vertShader);
    glCompileShader(fragShader);

    u32 program = glCreateProgram();
    glAttachShader(program, vertShader);
    glAttachShader(program, fragShader);
    glLinkProgram(program);

    checkForErrors(vertShader, fragShader, program);

    glDeleteShader(vertShader);
    glDeleteShader(fragShader);

    return program;
}

void clearScreen(Color c) {
    glClearColor(c.r, c.g, c.b, c.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

Box createBox(int numberOfBuffers) {
    Box b;
    glGenVertexArrays(1, &b.vao);
    b.bufferObjects.resize(numberOfBuffers);
    glGenBuffers(numberOfBuffers, b.bufferObjects.data());
    return b;
}

void pushData(u32 buff, u32 size, void* data) {
    glBindBuffer(GL_ARRAY_BUFFER, buff);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void formatData(Box b) {
    glBindVertexArray(b.vao);

    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, b.bufferObjects[0]);
    glVertexAttribPointer(0, 4, GL_FLOAT, false, 0, (void*)0);

    glBindVertexArray(0);
}

void drawCubes(u32 shaderProgram, Box b, u32 amountOfCubes, bool indexed) {
    glUseProgram(shaderProgram);
    glBindVertexArray(b.vao);
    if (indexed) {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, b.bufferObjects[1]);
        glDrawElements(GL_TRIANGLES, 12 * 3 * amountOfCubes, GL_UNSIGNED_INT, 0); // 12 triangles of each cube, 3 vertices of each triangle
    } else {
        glDrawArrays(GL_TRIANGLES, 0, 12 * 3 * amountOfCubes);
    }
    
}


int main()
{
    auto window = init(600, 600, "cube");
    auto shaderProgram = createShaderProgram();
    auto box = createBox(2);

    f32 data[] = {
         0.5f,  0.5f,  0.5f, 1.0f,
        -0.5f,  0.5f, -0.5f, 1.0f,
        -0.5f,  0.5f,  0.5f, 1.0f,
         0.5f, -0.5f, -0.5f, 1.0f,
        -0.5f, -0.5f, -0.5f, 1.0f,
         0.5f,  0.5f, -0.5f, 1.0f,
         0.5f, -0.5f,  0.5f, 1.0f,
        -0.5f, -0.5f,  0.5f, 1.0f,
    };

    u32 indices[] = {
        0, 1, 2,
        1, 3, 4,
        5, 6, 3,
        7, 3, 6,
        2, 4, 7,
        0, 7, 6,
        0, 5, 1,
        1, 5, 3,
        5, 0, 6,
        7, 4, 3,
        2, 1, 4,
        0, 2, 7,
    };

    pushData(box.bufferObjects[0], sizeof(data), data);
    pushData(box.bufferObjects[1], sizeof(indices), indices);
    formatData(box);

    glEnable(GL_CULL_FACE);
    glFrontFace(GL_CW);
    glCullFace(GL_BACK);

    while (!glfwWindowShouldClose(window))
    {
        clearScreen({BLACK});
        drawCubes(shaderProgram, box, 1, true);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}