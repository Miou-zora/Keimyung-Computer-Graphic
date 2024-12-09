#include "Plane.h"

#include <glm/gtc/constants.hpp>
#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>

#include <iostream>

Plane::Plane(unsigned int nbr_of_square)
{
	number_of_square = nbr_of_square;
	// Generate the vertex data
	generateMesh();

	for (auto &m : this->meshes)
		m.generateGlBuffers();
}

void Plane::draw(glm::mat4 projection, glm::mat4 view, ShaderProgram *shader) 
{
	for (auto &m : this->meshes)
		m.shader = shader;
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(-20.f, -1.f, -20.f));
	model = glm::rotate(model, glm::radians(90.f), glm::vec3(1.f, 0.f, 0.f));
	model = glm::scale(model, glm::vec3(40.0f, 40.0f, 40.0f));
	glm::mat4 mview = view * model;
	glm::mat4 mvp = projection * view * model;
	glm::mat4 imvp = glm::inverse(model);
	glm::mat3 nmat = glm::mat3(glm::transpose(imvp)); //normal matrix
	glUniformMatrix3fv(shader->uniform("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(nmat));
	glUniformMatrix4fv(shader->uniform("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(shader->uniform("MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
	for (auto &m : this->meshes) {
		m.draw();
	}
}

void Plane::generateMesh()
{
	using namespace glm;

	float square_scale = 1.f / static_cast<float>(number_of_square);

	for (int x = 0; x < number_of_square; x++) {
		for (int y = 0; y < number_of_square; y++) {
			std::vector<vec3> vertices;
			std::vector<vec3> normals;
			std::vector<vec<3, unsigned int>> triIndices;

			float f_x = static_cast<float>(x);
			float f_y = static_cast<float>(y);

			vertices = {
				vec3( f_x * square_scale, 		(f_y + 1) * square_scale, 0),
				vec3( (f_x + 1) * square_scale, (f_y + 1) * square_scale, 0),
				vec3( f_x * square_scale, 		f_y * square_scale, 0),
				vec3( (f_x + 1) * square_scale, f_y * square_scale, 0),
			};



			normals = {
				vec3(0, 0, -1),
				vec3(0, 0, -1),
				vec3(0, 0, -1),
				vec3(0, 0, -1)
			};

			triIndices = {
				{2, 0, 1},
				{2, 1, 3}
			};

			Mesh square;

			square.vertices = vertices;
			square.normals = normals;
			square.triIndices = triIndices;
			square.mat = Material(
				200.f, 							// Shiness
				glm::vec3(0.1f, 0.1f, 0.1f),	// Ambiant
				glm::vec3(1.f, 1.f, 1.f),		// Diffuse
				glm::vec3(1.f, 1.f, 1.f)		// Specular
			);
			if ((x + y) % 2 == 1) {
				square.mat.Kd = glm::vec3(0.1f, 0.1f, 0.1f);
			}
			meshes.push_back(square);
		}
	}
}

