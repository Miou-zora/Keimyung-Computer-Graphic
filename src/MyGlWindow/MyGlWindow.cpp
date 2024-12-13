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

	for (int i = 0; i < 5; i++) {
		m_shaderProgram->addUniform("Light[" + std::to_string(i) + "].Position");
		m_shaderProgram->addUniform("Light[" + std::to_string(i) + "].Intensity");
	}
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
	m_bunny = new Bunny();
}

struct Light {
	glm::vec4 Position;
	glm::vec3 Intensity;
};

void MyGlWindow::draw() {
	// Apply a static model matrix without rotation to stop the cube from spinning
	glm::mat4 view = glm::lookAt(m_viewer->getViewPoint(), m_viewer->getViewCenter(), m_viewer->getUpVector());
	glm::mat4 projection = glm::perspective(glm::radians(45.0f), (float)m_width / m_height, 0.1f, 100.0f);
	glm::mat4 projectionView = projection * view;
	
	

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_CULL_FACE); //Ebable Cull face
	glCullFace(GL_BACK);

	m_shaderProgram->use();

	Light light[] = {
		{glm::vec4(0, 0, 0, 1), glm::vec3(0.0f, 0.8f, 0.8f)},
		{glm::vec4(0, 0, 0, 1), glm::vec3(0.0f, 0.0f, 0.8f)},
		{glm::vec4(0, 0, 0, 1), glm::vec3(0.8f, 0.0f, 0.0f)},
		{glm::vec4(0, 0, 0, 1), glm::vec3(0.0f, 0.8f, 0.0f)},
		{glm::vec4(0, 0, 0, 1), glm::vec3(0.8f, 0.8f, 0.8f)}
	};

	float nbr_lights = 5.f;
	float scale = 2.f * glm::pi<float>() / nbr_lights;

	light[0].Position = glm::vec4( 5.f * cosf(scale * 0.f), 5.f, 5.f * sinf(scale * 0.f), 1.f);
	light[1].Position = glm::vec4( 5.f * cosf(scale * 1.f), 5.f, 5.f * sinf(scale * 1.f), 1.f);
	light[2].Position = glm::vec4( 5.f * cosf(scale * 2.f), 5.f, 5.f * sinf(scale * 2.f), 1.f);
	light[3].Position = glm::vec4( 5.f * cosf(scale * 3.f), 5.f, 5.f * sinf(scale * 3.f), 1.f);
	light[4].Position = glm::vec4( 5.f * cosf(scale * 4.f), 5.f, 5.f * sinf(scale * 4.f), 1.f);

	GLfloat shiness = 10;
	glm::vec3 Ka(0.2, 0.2, 0.2);
	glm::vec3 Kd(1, 1, 0);
	glm::vec3 Ks(1, 1, 1);
	
	for (int i = 0; i < 5; i++) {
		glUniform4fv(m_shaderProgram->uniform("Light[" + std::to_string(i) + "].Position"), 1, glm::value_ptr(light[i].Position));
		glUniform3fv(m_shaderProgram->uniform("Light[" + std::to_string(i) + "].Intensity"), 1, glm::value_ptr(light[i].Intensity));
	}
	glUniform3fv(m_shaderProgram->uniform("Material.Ka"), 1, glm::value_ptr(Ka));
	glUniform3fv(m_shaderProgram->uniform("Material.Kd"), 1, glm::value_ptr(Kd));
	glUniform3fv(m_shaderProgram->uniform("Material.Ks"), 1, glm::value_ptr(Ks));
	glUniform1fv(m_shaderProgram->uniform("Material.Shiness"), 1, &shiness);

	glUniform3fv(m_shaderProgram->uniform("CamPos"), 1, glm::value_ptr(m_viewer->getViewPoint()));

	// if (m_teapot) m_teapot->draw(projection, view, m_shaderProgram);
	// if (m_cow) m_cow->draw(projection, view, m_shaderProgram);
	// if (m_sphere) m_sphere->draw(projection, view, m_shaderProgram);
	// if (m_torus) m_torus->draw(projection, view, m_shaderProgram);
	// if (m_quad) m_quad->draw(projection, view, m_shaderProgram);
	if (m_plane) m_plane->draw(projection, view, m_shaderProgram);
	if (m_bunny) m_bunny->draw(projection, view, m_shaderProgram);

	m_shaderProgram->disable();
}
