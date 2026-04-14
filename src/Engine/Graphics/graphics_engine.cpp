#include <Engine/Graphics/graphics_engine.hpp>
#include <filesystem>
#include <fstream>
#include <sstream>

GraphicsEngine::GraphicsEngine() {
    window = new Window(800, 600, "EngineDemo");
    window->attachGlContext();
    gladLoadGL();
    glGenBuffers(1, &VBO);
    glGenVertexArrays(1, &VAO);
}

GraphicsEngine::~GraphicsEngine() {
    delete window;
}

void GraphicsEngine::createShaderProgram(const char* vertexShaderFilePath, const char* fragmentShaderFilePath) {

    std::ifstream vertex(vertexShaderFilePath);
    std::ifstream fragment(fragmentShaderFilePath);
    if (!vertex.is_open()) {/* Log error*/};
    if (!fragment.is_open()) {/* Log error*/};

    std::stringstream vert;
    std::stringstream frag;
    vert >> vertex.rdbuf();
    frag >> fragment.rdbuf();
    auto vertexShaderSource = vert.str().c_str();
    auto fragmentShaderSource = frag.str().c_str();
    vertex.close();
    fragment.close();
    

    vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);

    shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
}

void GraphicsEngine::clearScreen(Color c) {
    glClearColor(c.r, c.g, c.b, c.a);
    glClear(GL_COLOR_BUFFER_BIT);
}

void GraphicsEngine::setShaderProgram() {
    glUseProgram(shaderProgram);
}

void GraphicsEngine::setVAO() {
    glBindVertexArray(VAO);
    
}