#include <glad/glad.h>
#include <types.h>
#include <Engine/Window/window.hpp>

class GraphicsEngine
{
private:
    Window* window;
    u32 VAO;
    u32 VBO;
    u32 vertexShader;
    u32 fragmentShader;
    u32 shaderProgram;
public:
    GraphicsEngine();
    ~GraphicsEngine();
    void createShaderProgram(const char* vertexShaderFilePath, const char* fragmentShaderFilePath);
    void clearScreen(Color c);
    void setShaderProgram();
    void setVAO();
};