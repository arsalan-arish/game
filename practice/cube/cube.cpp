#include <glad/glad.h>
#include <glfw/glfw3.h>
#include <cstdio>
#include <types.h>

#define SHADER_FILE_MAXSIZE 5000


inline GLFWwindow* init(u16 width, u16 height, const char* title) {
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(width, height, title, nullptr, nullptr);
    glfwMakeContextCurrent(window);
    gladLoadGL();
    return window;
}

inline void checkForErrors(u32 vertexShader, u32 fragmentShader, u32 shaderProgram) {

    int success;
    char infoLog[512];

    glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::VERTEX::COMPILATION_FAILED\n%s\n", infoLog);
    }

    glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &success);
    if (!success) {
        glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
        printf("ERROR::SHADER::FRAGMENT::COMPILATION_FAILED\n%s\n", infoLog);
    }

    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success) {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        printf("ERROR::SHADER::PROGRAM::LINKING_FAILED\n%s\n", infoLog);
    }
}

inline u32 createShaderProgram() {

    static char vertSource[SHADER_FILE_MAXSIZE];
    static char fragSource[SHADER_FILE_MAXSIZE];

    FILE* vert = fopen("practice/cube/shader.vert", "r");
    FILE* frag = fopen("practice/cube/shader.frag", "r");

    fgets(vertSource, SHADER_FILE_MAXSIZE, vert);
    fgets(fragSource, SHADER_FILE_MAXSIZE, frag);
    
    char* vertSourcePtr = vertSource;
    char* fragSourcePtr = fragSource;

    u32 vertShader = glCreateShader(GL_VERTEX_SHADER);
    u32 fragShader = glCreateShader(GL_FRAGMENT_SHADER);

    glShaderSource(vertShader, 1, &vertSourcePtr, NULL);
    glShaderSource(fragShader, 1, &fragSourcePtr, NULL);
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

inline Box createBox() {
    Box b;
    glGenBuffers(1, &b.vbo);
    glGenVertexArrays(1, &b.vao);
    // Save the vbo to the vao
    glBindVertexArray(b.vao);
    glBindBuffer(GL_ARRAY_BUFFER, b.vbo);
    glBindVertexArray(0);
    return b;
}

inline void pushData(Box b, u32 size, void* data) {
    glBindVertexArray(b.vao);

    glBindBuffer(GL_ARRAY_BUFFER, b.vbo);
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    // Format and set slot (attribute index)
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, (void*) 0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);
}

inline void clearScreen(Color c) {
    glClearColor(c.r, c.g, c.b, c.a);
    glClear(GL_COLOR_BUFFER_BIT);
}


int main()
{
    auto window = init(800, 600, "Cube");
    auto shaderProgram = createShaderProgram();
    auto box = createBox();

    f32 vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
    };

    pushData(
        box,
        sizeof(vertices),
        vertices
    );

    while (!glfwWindowShouldClose(window))
    {
        clearScreen({0.0f, 0.1f, 0.5f, 1.0f});

        glUseProgram(shaderProgram);
        glBindVertexArray(box.vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}