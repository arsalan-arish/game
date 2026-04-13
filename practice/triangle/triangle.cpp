#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <include/types.h>

const char* vertexShaderSource = 
"#version 330 core\n"
"layout (location = 0) in vec3 aPos;\n"
"void main() {\n"
"\tgl_Position = vec4(aPos, 1.0);\n"
"}\0";

const char* fragmentShaderSource = 
"#version 330 core\n"
"out vec4 FragColor;\n"
"void main() {\n"
"\tFragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);\n"
"}\0";

int main()
{
    // Initialize the window, opengl context and glad
    glfwInit();
    GLFWwindow* window = glfwCreateWindow(800, 600, "Triangle", nullptr, nullptr);
    glfwMakeContextCurrent(window);
    gladLoadGL();

    f32 vertices[] = {
        -0.5f, -0.5f, 0.0f,
        0.5f, -0.5f, 0.0f,
        0.0f, 0.5f, 0.0f,
    };

    u32 vbo, vao;
    glGenBuffers(1, &vbo);
    glGenVertexArrays(1, &vao);

    glBindVertexArray(vao);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(0);
    glBindVertexArray(0);

    // Load and compile the shaders
    u32 vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    u32 fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    // Create and link shader program
    u32 shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    // Once linked, you can delete the individual shader objects to save memory
    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    // Rendering loop
    while (!glfwWindowShouldClose(window))
    {
        glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUseProgram(shaderProgram);

        glBindVertexArray(vao);
        glDrawArrays(GL_TRIANGLES, 0, 3);

        glfwSwapBuffers(window);
        glfwPollEvents();
    }
}

//! Checking for compilation errors
// int success;
// char infoLog[512];
// glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &success);
// if (!success) {
//     glGetShaderInfoLog(vertexShader, 512, NULL, infoLog);
//     std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
// }

//! Checking for linking errors
// glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
// if (!success) {
//     glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
//     std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
// }