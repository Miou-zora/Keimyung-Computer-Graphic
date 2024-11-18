#include "GL/gl3w.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include "MyGlWindow.h"

MyGlWindow* win = NULL;

//to control the viewpoint
bool lbutton_down = false;
bool rbutton_down = false;
bool mbutton_down = false;
double m_lastMouseX = 0.0;
double m_lastMouseY = 0.0;
double cx = 0.0, cy = 0.0;



// Callback to handle window resizing
void window_size_callback(GLFWwindow* window, int width, int height)
{
    win->setSize(width, height);
    win->setAspect(width / (float)height);
}

// Callback to handle keyboard events
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GLFW_TRUE);
}

// Callback to track the cursor position
static void cursor_pos_callback(GLFWwindow* window, double xpos, double ypos)
{
    cx = xpos;
    cy = ypos;
}

// Callback to handle mouse button presses
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    if (action == GLFW_PRESS) {
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);
        m_lastMouseX = xpos;
        m_lastMouseY = ypos;
    }

    if (button == GLFW_MOUSE_BUTTON_LEFT) {
        lbutton_down = (action == GLFW_PRESS);
    }
    else if (button == GLFW_MOUSE_BUTTON_RIGHT) {
        rbutton_down = (action == GLFW_PRESS);
    }
    else if (button == GLFW_MOUSE_BUTTON_MIDDLE) {
        mbutton_down = (action == GLFW_PRESS);
    }
}

// Function to handle mouse dragging interactions
void mouseDragging(double width, double height)
{
    if (lbutton_down) {
        float fractionChangeX = static_cast<float>(cx - m_lastMouseX) / static_cast<float>(width);
        float fractionChangeY = static_cast<float>(m_lastMouseY - cy) / static_cast<float>(height);
        win->m_viewer->rotate(fractionChangeX, fractionChangeY);
    }
    else if (mbutton_down) {
        float fractionChangeY = static_cast<float>(m_lastMouseY - cy) / static_cast<float>(height);
        win->m_viewer->zoom(fractionChangeY);
    }
    else if (rbutton_down) {
        float fractionChangeX = static_cast<float>(cx - m_lastMouseX) / static_cast<float>(width);
        float fractionChangeY = static_cast<float>(m_lastMouseY - cy) / static_cast<float>(height);
        win->m_viewer->translate(-fractionChangeX, -fractionChangeY, true);
    }
    m_lastMouseX = cx;
    m_lastMouseY = cy;
}

// Main function
int main()
{
    // Initialize the library
    if (!glfwInit()) {
        // Initialization failed
        return -1;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    int width = 800;
    int height = 800;

    // Create a windowed mode window and its OpenGL context
    GLFWwindow* window = glfwCreateWindow(width, height, "OpenGL Framework", NULL, NULL);
    if (!window) {
        glfwTerminate();
        return -1;
    }

    glfwMakeContextCurrent(window);
    if (gl3wInit()) {
        fprintf(stderr, "failed to initialize OpenGL\n");
        return -1;
    }

    if (!gl3wIsSupported(4, 2)) {
        fprintf(stderr, "OpenGL 4.2 not supported\n");
        return -1;
    }

    glfwSwapInterval(1); // Enable vsync

    win = new MyGlWindow(width, height);

    // Set GLFW callback functions
    glfwSetWindowSizeCallback(window, window_size_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetCursorPosCallback(window, cursor_pos_callback);
    glfwSetKeyCallback(window, key_callback);

    // Loop until the user closes the window
    while (!glfwWindowShouldClose(window)) {
        // Clear buffer and draw
        win->draw();

        // Swap front and back buffers
        glfwSwapBuffers(window);

        // Poll for and process events
        glfwPollEvents();

        // Handle mouse dragging for viewpoint adjustments
        mouseDragging(width, height);
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
