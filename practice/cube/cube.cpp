#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <cstdio>
#include <types.h>
#include <string>
#include <fstream>
#include <sstream>


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
    Box<numberOfBuffers> b;
    glGenBuffers(numberOfBuffers, &b.bufferObjects[0]);
    glGenVertexArrays(1, &b.vao);
    return b;
}

void pushData(u32 buff, u32 size, void* data) {
    glBindBuffer(GL_ARRAY_BUFFER, buff);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
}

void formatData(Box<1> b) {
    glBindVertexArray(b.vao);

    glBindBuffer(GL_ARRAY_BUFFER, b.bufferObjects[0]);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, false, 8*sizeof(f32), (void*)0);
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 4, GL_FLOAT, false, 8*sizeof(f32), (void*) (4*sizeof(float)));

    glBindVertexArray(0);
}

void drawTriangle(u32 shaderProgram, u32 VAO, u32 amountOfTriangles) {
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawArrays(GL_TRIANGLES, 0, 3 * amountOfTriangles);
}



int main()
{
    auto window = init(800, 600, "Cube");
    auto shaderProgram = createShaderProgram();
    auto box = createBox(1);

    f32 data[] = {
        -0.5f, -0.5f, 0.0f, 1.0f,
         1.0f,  0.0f, 0.0f, 1.0f,

         0.5f, -0.5f, 0.0f, 1.0f,
         0.0f,  1.0f, 0.0f, 1.0f,

         0.0f, 0.5f, 0.0f, 1.0f,
         0.0f, 0.0f, 1.0f, 1.0f,
    };

    pushData(box.vbo, sizeof(data), data);
    formatData(box);

    while (!glfwWindowShouldClose(window))
    {
        clearScreen({0.0f, 0.0f, 0.0f, 1.0f});
        drawTriangle(shaderProgram, box.vao, 1);
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}