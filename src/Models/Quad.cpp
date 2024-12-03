#include "Quad.h"

#include <glm//gtc/constants.hpp>
#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>


#include <iostream>

Quad::Quad()
{
	// Generate the vertex data
	generateMesh();

	mesh.generateGlBuffers();
}

void Quad::draw(glm::mat4 projection, glm::mat4 view, ShaderProgram *shader) 
{
	this->mesh.shader = shader;
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 0.0f, 0.0f));
	model = glm::scale(model, glm::vec3(10.0f, 10.0f, 10.0f));
	glm::mat4 mview = view * model;
	glm::mat4 mvp = projection * view * model;
	glm::mat4 imvp = glm::inverse(model);
	glm::mat3 nmat = glm::mat3(glm::transpose(imvp)); //normal matrix
	glUniformMatrix3fv(shader->uniform("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(nmat));
	glUniformMatrix4fv(shader->uniform("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(shader->uniform("MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
	mesh.draw();
}

void Quad::generateMesh()
{
	using namespace glm;

	std::vector<vec3> vertices;
	std::vector<vec3> normals;
	std::vector<vec<3, unsigned int>> triIndices;

	vertices = {
		vec3(-1,  1, 0),
		vec3( 1,  1, 0),
		vec3(-1, -1, 0),
		vec3( 1, -1, 0)
	};

	normals = {
		vec3(0, 0, 1),
		vec3(0, 0, 1),
		vec3(0, 0, 1),
		vec3(0, 0, 1)
	};

	triIndices = {
		{2, 0, 1},
		{2, 1, 3}
	};

	mesh.vertices = vertices;
	mesh.normals = normals;
	mesh.triIndices = triIndices;
	mesh.mat = Material(
		10, 						// Shiness
		glm::vec3(0.2, 0.2, 0.2),	// Ambiant
		glm::vec3(1, 0, 0),			// Diffuse
		glm::vec3(1, 1, 1)			// Specular
	);
}

