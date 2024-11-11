#include <iostream>
#include "GL/gl3w.h"
#include <string>
#include "Loader.h"

class MyGlWindow {
public:
	MyGlWindow(int w, int h);
	void draw();
private:
	ShaderProgram* m_shaderProgram;
	int m_width;
	int m_height;
	GLuint vaoHandle;
	GLuint vbo_vertexPosition;
	GLuint vbo_vertexColor;
	GLuint vbo_vertex; // vbo: vertex position + vertex color
	void setupBuffer();
};