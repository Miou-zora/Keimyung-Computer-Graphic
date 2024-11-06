#include "MyGlWindow.h"
MyGlWindow::MyGlWindow(int w, int h)
{
	m_width = w;
	m_height = h;
	setupBuffer();
}

void MyGlWindow::setupBuffer()
{
	const float vertexPositions[] = {
		-0.2f, -0.2f, 0.0f, 1.0f,
		0.2f, -0.2f, 0.0f, 1.0f,
		0.0f, 0.2f, 0.0f, 1.0f
	};

	const float vertexColors[] = {
		1.0f, 0.0f, 0.0f,
		0.0f, 1.0f, 0.0f,
		0.0f, 0.0f, 1.0f
	};

	m_shaderProgram = new ShaderProgram();
	m_shaderProgram->initFromFiles("shaders/simple.vert", "shaders/simple.frag");

	glGenVertexArrays(1, &vaoHandle);
	glBindVertexArray(vaoHandle);

	glGenBuffers(1, &vbo_vertexPosition);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertexPosition);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
	glVertexAttribPointer(
		0, // attribute pointer
		4, // nbr of data per vertex
		GL_FLOAT, // type
		GL_FALSE, // normalize
		0, // offset
		0); // stride
	glEnableVertexAttribArray(0); // attribute nbr

	glGenBuffers(1, &vbo_vertexColor);
	glBindBuffer(GL_ARRAY_BUFFER, vbo_vertexColor);
	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColors), vertexColors, GL_STATIC_DRAW);
	glVertexAttribPointer(
		1, // attribute pointer
		3, // nbr of data per vertex
		GL_FLOAT, // type
		GL_FALSE, // normalize
		0, // offset
		0); // stride
	glEnableVertexAttribArray(1); // attribute nbr

	glBindVertexArray(0);
}
void MyGlWindow::draw()
{
	glClearColor(0.2f, 0.2f, 0.2f, 1.0f); // bg color
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST); // enable depth

	// first two param: starting point, next two param: width and height
	glViewport(0, 0, m_width, m_height); // set up the screen space

	// draw
	m_shaderProgram->use(); // call shader
	// binding vao
	glBindVertexArray(vaoHandle); // pick a vao for drawing
	// first: primitive, second: starting index, third: nbr of vertices
	glDrawArrays(GL_TRIANGLES, 0, 3);
	m_shaderProgram->disable(); // close shader
}
