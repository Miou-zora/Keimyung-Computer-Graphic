#include "MyGlWindow.h"
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <GLFW/glfw3.h>

MyGlWindow::MyGlWindow(int w, int h) : m_width(w), m_height(h) {
	m_cow = nullptr;
	m_sphere = nullptr;
	m_torus = nullptr;
	m_teapot = nullptr;
	m_viewer = new Viewer(glm::vec3(5, 5, 5), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0), 45.0f, (float)w / h);
	setupBuffer();
}

void MyGlWindow::setupBuffer() {
	m_shaderProgram = new ShaderProgram();
	m_shaderProgram->initFromFiles("shaders/simple.vert", "shaders/simple.frag");

	// Add uniforms
	m_shaderProgram->addUniform("MVP");
	m_shaderProgram->addUniform("ModelMatrix"); //View*Model : mat4
	m_shaderProgram->addUniform("NormalMatrix"); //Refer next slide : mat3

	m_shaderProgram->addUniform("Light.Position");
	m_shaderProgram->addUniform("Light.Ia");
	m_shaderProgram->addUniform("Light.Id");
	m_shaderProgram->addUniform("Light.Is");
	m_shaderProgram->addUniform("Material.Ka");
	m_shaderProgram->addUniform("Material.Kd");
	m_shaderProgram->addUniform("Material.Ks");
	m_shaderProgram->addUniform("Material.Shiness");

	m_shaderProgram->addUniform("CamPos");

	initialize();
}

void MyGlWindow::initialize() {
	m_cow = new Cow();
	m_sphere = new Sphere();
	m_torus = new VBOTorus();
	m_teapot = new VBOTeapot();
	m_quad = new Quad();
	m_plane = new Plane();
}

void MyGlWindow::draw() {
	// Apply a static model matrix without rotation to stop the cube from spinning
	glm::mat4 view = glm::lookAt(m_viewer->getViewPoint(), m_viewer->getViewCenter(), m_viewer->getUpVector());
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)m_width / m_height, 0.1f, 100.0f);
	glm::mat4 projectionView = projection * view;
	
	

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);

	m_shaderProgram->use();

	glm::vec4 lightPos(50, 50, 50, 1);
	glm::vec3 Ia(0.9, 0.9, 0.9);
	glm::vec3 Id(0.9, 0.9, 0.9);
	glm::vec3 Is(0.9, 0.9, 0.9);
	GLfloat shiness = 10;
	glm::vec3 Ka(0.2, 0.2, 0.2);
	glm::vec3 Kd(1, 1, 0);
	glm::vec3 Ks(1, 1, 1);

	glUniform4fv(m_shaderProgram->uniform("Light.Position"), 1, glm::value_ptr(lightPos));
	glUniform3fv(m_shaderProgram->uniform("Light.Ia"), 1, glm::value_ptr(Ia));
	glUniform3fv(m_shaderProgram->uniform("Light.Id"), 1, glm::value_ptr(Id));
	glUniform3fv(m_shaderProgram->uniform("Light.Is"), 1, glm::value_ptr(Is));
	glUniform3fv(m_shaderProgram->uniform("Material.Ka"), 1, glm::value_ptr(Ka));
	glUniform3fv(m_shaderProgram->uniform("Material.Kd"), 1, glm::value_ptr(Kd));
	glUniform3fv(m_shaderProgram->uniform("Material.Ks"), 1, glm::value_ptr(Ks));
	glUniform1fv(m_shaderProgram->uniform("Material.Shiness"), 1, &shiness);

	glUniform3fv(m_shaderProgram->uniform("CamPos"), 1, glm::value_ptr(m_viewer->getViewPoint()));

	if (m_teapot) m_teapot->draw(projection, view, m_shaderProgram);
	if (m_cow) m_cow->draw(projection, view, m_shaderProgram);
	if (m_sphere) m_sphere->draw(projection, view, m_shaderProgram);
	if (m_torus) m_torus->draw(projection, view, m_shaderProgram);
	// if (m_quad) m_quad->draw(projection, view, m_shaderProgram);
	if (m_plane) m_plane->draw(projection, view, m_shaderProgram);

	m_shaderProgram->disable();
}
