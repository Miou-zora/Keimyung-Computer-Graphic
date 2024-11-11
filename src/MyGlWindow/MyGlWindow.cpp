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

	// combining vertex position + vertex color
	const float vertices[] = {
		-1.0f, 1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f,		0.0f, 1.0f, 0.0f,
		-1.0f, -1.0f, 0.0f, 1.0f,		1.0f, 0.0f, 0.0f,
		-1.0f, 1.0f, 0.0f, 1.0f,		1.0f, 1.0f, 1.0f,
		1.0f, 1.0f, 0.0f, 1.0f,			0.0f, 0.0f, 1.0f,
		1.0f, -1.0f, 0.0f, 1.0f,		0.0f, 1.0f, 0.0f
	};

	m_shaderProgram = new ShaderProgram();
	m_shaderProgram->initFromFiles("shaders/simple.vert", "shaders/simple.frag");
	{
		// glGenVertexArrays(1, &vaoHandle);
		// glBindVertexArray(vaoHandle);

		// glGenBuffers(1, &vbo_vertexPosition);
		// glBindBuffer(GL_ARRAY_BUFFER, vbo_vertexPosition);
		// glBufferData(GL_ARRAY_BUFFER, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
		// glVertexAttribPointer(
		// 	0, // attribute pointer
		// 	4, // nbr of data per vertex
		// 	GL_FLOAT, // type
		// 	GL_FALSE, // normalize
		// 	0, // stride
		// 	0); // offset
		// glEnableVertexAttribArray(0); // attribute nbr

		// glGenBuffers(1, &vbo_vertexColor);
		// glBindBuffer(GL_ARRAY_BUFFER, vbo_vertexColor);
		// glBufferData(GL_ARRAY_BUFFER, sizeof(vertexColors), vertexColors, GL_STATIC_DRAW);
		// glVertexAttribPointer(
		// 	1, // attribute pointer
		// 	3, // nbr of data per vertex
		// 	GL_FLOAT, // type
		// 	GL_FALSE, // normalize
		// 	0, // stride
		// 	0); // offset
		// glEnableVertexAttribArray(1); // attribute nbr

		// glBindVertexArray(0);
	}
	{ // Creating a single VBO case
		// glGenVertexArrays(1, &vaoHandle);
		// glBindVertexArray(vaoHandle);
		// // Create a single vbo
		// glGenBuffers(1, &vbo_vertex);
		// glBindBuffer(GL_ARRAY_BUFFER, vbo_vertex);
		// glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
		// // Below function tell GPU how to interpretate the data
		// glVertexAttribPointer(
		// 	0, // attribute pointer
		// 	4, // nbr of data per vertex
		// 	GL_FLOAT, // type
		// 	GL_FALSE, // normalize
		// 	sizeof(float) * 7, // stride
		// 	0); // offset
		// glEnableVertexAttribArray(0);

		// glVertexAttribPointer(
		// 	1, // attribute pointer
		// 	3, // nbr of data per vertex
		// 	GL_FLOAT, // type
		// 	GL_FALSE, // normalize
		// 	sizeof(float) * 7, // stride
		// 	(void*)(sizeof(float) * 4) // offset
		// );
		// glEnableVertexAttribArray(1);
		// glBindVertexArray(0);
	}
	{ // 2. Using DSA (two VBO first)
		// glCreateVertexArrays(1, &vaoHandle);
		// glCreateBuffers(1, &vbo_vertexPosition);
		// glCreateBuffers(1, &vbo_vertexColor);
		// // Allocation memory of VBO
		// glNamedBufferData(vbo_vertexPosition, sizeof(vertexPositions), vertexPositions, GL_STATIC_DRAW);
		// glVertexArrayVertexBuffer(vaoHandle, 0, vbo_vertexPosition, 0, sizeof(float) * 4);

		// // Allocation memory of VBO
		// glNamedBufferData(vbo_vertexColor, sizeof(vertexColors), vertexColors, GL_STATIC_DRAW);
		// glVertexArrayVertexBuffer(vaoHandle, 1, vbo_vertexColor, 0, sizeof(float) * 3);

		// glVertexArrayAttribFormat(vaoHandle,
		// 	0,				// Attrib ptr
		// 	4,				// Number of data
		// 	GL_FLOAT,
		// 	GL_FALSE,
		// 	0);
		// glVertexAttribBinding(0, 0); // Binding index attri number
		// glEnableVertexArrayAttrib(vaoHandle, 0);

		// glVertexArrayAttribFormat(vaoHandle,
		// 	1,				// Attrib ptr
		// 	3,				// Number of data
		// 	GL_FLOAT,
		// 	GL_FALSE,
		// 	0);
		// glVertexAttribBinding(1, 1); // Binding index attri number
		// glEnableVertexArrayAttrib(vaoHandle, 1);
	}
	{ // 2. Using DSA (one VBO)
		glCreateVertexArrays(1, &vaoHandle);
		glCreateBuffers(1, &vbo_vertex);
		// Allocation memory of VBO
		glNamedBufferData(vbo_vertex, sizeof(vertices), vertices, GL_STATIC_DRAW);
		glVertexArrayVertexBuffer(vaoHandle, 0, vbo_vertex, 0, sizeof(float) * 7);

		glVertexArrayAttribFormat(vaoHandle,
			0,				// Attrib ptr
			4,				// Number of data
			GL_FLOAT,
			GL_FALSE,
			0);
		glVertexArrayAttribBinding(vaoHandle, 0, 0); // Binding index attri number
		glEnableVertexArrayAttrib(vaoHandle, 0);

		glVertexArrayAttribFormat(vaoHandle,
			1,				// Attrib ptr
			3,				// Number of data
			GL_FLOAT,
			GL_FALSE,
			sizeof(float) * 4);
		glVertexArrayAttribBinding(vaoHandle, 1, 0); // Binding index attri number
		glEnableVertexArrayAttrib(vaoHandle, 1);
		glBindVertexArray(0);
	}
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
	glDrawArrays(GL_TRIANGLES, 0, 6);
	m_shaderProgram->disable(); // close shader
}
