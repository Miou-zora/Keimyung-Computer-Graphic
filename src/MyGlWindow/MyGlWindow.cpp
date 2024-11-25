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
	m_shaderProgram->addUniform("LightLocation"); //Light Position : vec4
	m_shaderProgram->addUniform("Kd"); //Diffuse Object Color :vec3
	m_shaderProgram->addUniform("Ld"); //Diffuse Light Color : vec3
	m_shaderProgram->addUniform("ModelViewMatrix"); //View*Model : mat4
	m_shaderProgram->addUniform("NormalMatrix"); //Refer next slide : mat3
	m_shaderProgram->addUniform("MVP");

	initialize();
}

void MyGlWindow::initialize() {
	m_cube = new Cow();
}

void MyGlWindow::draw() {
	// Apply a static model matrix without rotation to stop the cube from spinning
	glm::mat4 model = glm::mat4(1.0f); // No rotation
	glm::mat4 view = glm::lookAt(m_viewer->getViewPoint(), m_viewer->getViewCenter(), m_viewer->getUpVector());
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)m_width / m_height, 0.1f, 100.0f);

	glm::mat4 mview = view * model;
	glm::mat4 mvp = projection * view * model;
	glm::vec4 lightPos(50, 50, 50, 1); //light position
	glm::vec3 Kd(1, 1, 0); //Diffuse Object Color
	glm::vec3 Ld(1, 1, 1); //Diffuse Light Color
	glm::mat4 imvp = glm::inverse(mview);
	glm::mat3 nmat = glm::mat3(glm::transpose(imvp)); //normal matrix

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	m_shaderProgram->use();

	glUniform4fv(m_shaderProgram->uniform("LightLocation"), 1, glm::value_ptr(lightPos));
	//for LightPos
	glUniform3fv(m_shaderProgram->uniform("Kd"), 1, glm::value_ptr(Kd));
	//for Kd
	glUniform3fv(m_shaderProgram->uniform("Ld"), 1, glm::value_ptr(Ld));
	//for Ld
	glUniformMatrix4fv(m_shaderProgram->uniform("ModelViewMatrix"), 1, GL_FALSE, glm::value_ptr(mview));
	//modelView
	glUniformMatrix3fv(m_shaderProgram->uniform("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(nmat));
	//normalMatrix
	glUniformMatrix4fv(m_shaderProgram->uniform("MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
	//mvp

	if (m_cube) m_cube->draw();

	m_shaderProgram->disable();
}
