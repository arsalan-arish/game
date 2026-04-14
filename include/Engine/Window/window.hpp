#include <glfw/glfw3.h>
#include <types.h>

class Window
{
private:
    GLFWwindow* window = nullptr;
public:
    Window(u16 width, u16 height, const char* title);
    ~Window();
    void attachGlContext();
    void present();
    void pollEvents();
    bool closed();
};