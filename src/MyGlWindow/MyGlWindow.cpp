#include "MyGlWindow.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

MyGlWindow::MyGlWindow(int w, int h) : m_width(w), m_height(h) {
	m_cube = nullptr;
	m_viewer = new Viewer(glm::vec3(5, 5, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), 45.0f, (float)w / h);
	setupBuffer();
}

void MyGlWindow::setupBuffer() {
	m_shaderProgram = new ShaderProgram();
	m_shaderProgram->initFromFiles("shaders/simple.vert", "shaders/simple.frag");

	// Add uniforms
	m_shaderProgram->addUniform("model");
	m_shaderProgram->addUniform("view");
	m_shaderProgram->addUniform("projection");

	initialize();
}

void MyGlWindow::initialize() {
	m_cube = new ColorCube();
}

void MyGlWindow::draw() {
	// Apply a static model matrix without rotation to stop the cube from spinning
	glm::mat4 model = glm::mat4(1.0f); // No rotation
	glm::mat4 view = glm::lookAt(m_viewer->getViewPoint(), m_viewer->getViewCenter(), m_viewer->getUpVector());
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)m_width / m_height, 0.1f, 100.0f);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	m_shaderProgram->use();
	glUniformMatrix4fv(m_shaderProgram->uniform("model"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(m_shaderProgram->uniform("view"), 1, GL_FALSE, glm::value_ptr(view));
	glUniformMatrix4fv(m_shaderProgram->uniform("projection"), 1, GL_FALSE, glm::value_ptr(projection));

	if (m_cube) m_cube->draw();

	m_shaderProgram->disable();
}
