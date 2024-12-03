#include "Sphere.h"

#include <glm//gtc/constants.hpp>
#include <glm/vec3.hpp>
#include <glm/mat3x3.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
Sphere::Sphere() : Sphere(1.0f, 20, 20)
{

}


Sphere::~Sphere()
{
	glDeleteBuffers(1, &VBO_position);
	glDeleteBuffers(1, &VBO_normal);
	glDeleteBuffers(1, &IBO);
	glDeleteVertexArrays(1, &VAO);
}

#include <iostream>

Sphere::Sphere(float rad, GLuint sl, GLuint st) :
radius(rad), slices(sl), stacks(st)
{
	nVerts = (slices + 1) * (stacks + 1);
	elements = (slices * 2 * (stacks - 1)) * 3;

	// Verts
	float * v = new float[3 * nVerts];
	// Normals
	float * n = new float[3 * nVerts];  
	// Tex coords
	float * tex = new float[2 * nVerts];    //we don't use it now
	// Index
	unsigned int * el = new unsigned int[elements];  //index

	// Generate the vertex data
	generateVerts(v, n, tex, el);

	// 0.25, 0.25, 0.25, 1.0,
	// 0.4, 0.4, 0.4, 1.0,
	// 0.774597,0.774597, 0.774597, 1.0,
	// 76.8,
	mat.Shiness = 76.8f;
	mat.Ka = glm::vec3(0.25f, 0.25f, 0.25f);
	mat.Kd = glm::vec3(0.4f, 0.4f, 0.4f);
	mat.Ks = glm::vec3(0.774597f, 0.774597f, 0.774597f);


	//create vao, vbo and ibo here... (We didn't use std::vector here...)
	glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    // Vertex positions VBO
    glGenBuffers(1, &VBO_position);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_position);
    glBufferData(GL_ARRAY_BUFFER, 3 * nVerts * sizeof(float), v, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // Vertex Normal VBO
    glGenBuffers(1, &VBO_normal);
    glBindBuffer(GL_ARRAY_BUFFER, VBO_normal);
    glBufferData(GL_ARRAY_BUFFER,  3 * nVerts * sizeof(float), n, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    // Element indices buffer
    glGenBuffers(1, &IBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, elements * sizeof(unsigned int), el, GL_STATIC_DRAW);

    glBindVertexArray(0);
	std::cout << "Sphere created" << std::endl;

	delete[] v;
	delete[] n;
	delete[] el;
	delete[] tex;

}

void Sphere::draw(glm::mat4 projection, glm::mat4 view, ShaderProgram *shader) 
{
	glUniform3fv(shader->uniform("Material.Ka"), 1, glm::value_ptr(mat.Ka));
    glUniform3fv(shader->uniform("Material.Kd"), 1, glm::value_ptr(mat.Kd));
    glUniform3fv(shader->uniform("Material.Ks"), 1, glm::value_ptr(mat.Ks));
    glUniform1fv(shader->uniform("Material.Shiness"), 1, &mat.Shiness);
	glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(4.0f, 0.0f, 0.0f));
	glm::mat4 mview = view * model;
	glm::mat4 mvp = projection * view * model;
	glm::mat4 imvp = glm::inverse(model);
	glm::mat3 nmat = glm::mat3(glm::transpose(imvp)); //normal matrix
	glUniformMatrix3fv(shader->uniform("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(nmat));
	glUniformMatrix4fv(shader->uniform("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(shader->uniform("MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, (slices * 2 * (stacks - 1)) * 3, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void Sphere::generateVerts(float * verts, float * norms, float * tex,
	unsigned int * el)
{
	// Generate positions and normals
	GLfloat theta, phi;
	GLfloat thetaFac = glm::two_pi<float>() / slices;
	GLfloat phiFac = glm::pi<float>() / stacks;
	GLfloat nx, ny, nz, s, t;
	GLuint idx = 0, tIdx = 0;
	for (GLuint i = 0; i <= slices; i++) {
		theta = i * thetaFac;
		s = (GLfloat)i / slices;
		for (GLuint j = 0; j <= stacks; j++) {
			phi = j * phiFac;
			t = (GLfloat)j / stacks;
			nx = sinf(phi) * cosf(theta);
			ny = sinf(phi) * sinf(theta);
			nz = cosf(phi);
			verts[idx] = radius * nx; verts[idx + 1] = radius * ny; verts[idx + 2] = radius * nz;
			norms[idx] = nx; norms[idx + 1] = ny; norms[idx + 2] = nz;
			idx += 3;

			tex[tIdx] = s;
			tex[tIdx + 1] = t;
			tIdx += 2;
		}
	}

	// Generate the element list
	idx = 0;
	for (GLuint i = 0; i < slices; i++) {
		GLuint stackStart = i * (stacks + 1);
		GLuint nextStackStart = (i + 1) * (stacks + 1);
		for (GLuint j = 0; j < stacks; j++) {
			if (j == 0) {
				el[idx] = stackStart;
				el[idx + 1] = stackStart + 1;
				el[idx + 2] = nextStackStart + 1;
				idx += 3;
			}
			else if (j == stacks - 1) {
				el[idx] = stackStart + j;
				el[idx + 1] = stackStart + j + 1;
				el[idx + 2] = nextStackStart + j;
				idx += 3;
			}
			else {
				el[idx] = stackStart + j;
				el[idx + 1] = stackStart + j + 1;
				el[idx + 2] = nextStackStart + j + 1;
				el[idx + 3] = nextStackStart + j;
				el[idx + 4] = stackStart + j;
				el[idx + 5] = nextStackStart + j + 1;
				idx += 6;
			}
		}
	}
}

int Sphere::getVertexArrayHandle() 
{
	return this->VAO;
}

