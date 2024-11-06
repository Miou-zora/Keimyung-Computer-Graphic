// CMakeProject1.cpp : Defines the entry point for the application.
//

#include "GL/gl3w.h"
#include "GLFW/glfw3.h"
#include "glm/glm.hpp"

#include "MyGlWindow.h"

MyGlWindow* win = NULL;

using namespace std;

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
		glfwSetWindowShouldClose(window, GLFW_TRUE);
}

int main()
{
	/* Initialize the library */
	if (!glfwInit())
	{
		// Initialization failed
	}
	glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
	glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
	glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

	int width = 800;
	int height = 800;
	/* Create a windowed mode window and its OpenGL context */
	GLFWwindow *window = glfwCreateWindow(width, height, "OpenGL FrameWork", NULL, NULL);
	if (!window)
	{
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
	glfwSwapInterval(1); //enable vsync

	win = new MyGlWindow(width, height);

	glfwSetKeyCallback(window, key_callback);
	glfwSetWindowTitle(window, "MyOpenGLWindow");
	while (!glfwWindowShouldClose(window))
	{ //////////////////////////////
		win->draw();
		/* Swap front and back buffers */
		glfwSwapBuffers(window);
		/* Poll for and process events */
		glfwPollEvents();
	}
	glfwDestroyWindow(window);
	glfwTerminate();
	return 0;
}
