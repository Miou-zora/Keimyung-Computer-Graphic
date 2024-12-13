// ColorCube.cpp
#include "Bunny.h"
#include <vector>
#include <GL/gl.h>
#include <cstdio>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

Bunny::Bunny() {
    setup();
}

Bunny::~Bunny() {
    glDeleteBuffers(1, &vbo_vertices);
    glDeleteBuffers(1, &vbo_normals);
    glDeleteBuffers(1, &ibo_elements);
    glDeleteVertexArrays(1, &vaoHandle);
}

// struct ModelVertex {
//     glm::vec3 position;
//     glm::vec3 normal;
//     glm::vec2 uv;
// };

void Bunny::setup() {
    int nvertices = 72378;
    int ntriangles = 432138;

    mat.Shiness = 51.2f;
    mat.Ka = glm::vec3(0.24725f, 0.1995f, 0.0745f);
    mat.Kd = glm::vec3(0.75164f, 0.60648f, 0.22648f);
    mat.Ks = glm::vec3(0.628281f, 0.555802f, 0.366065f);


    glCreateVertexArrays(1, &this->vaoHandle);
    glCreateBuffers(1, &this->vbo_vertices);
    glCreateBuffers(1, &ibo_elements);

    // Allocation memory of VBO
    glNamedBufferData(this->vbo_vertices, sizeof(modelVertices), modelVertices, GL_STATIC_DRAW);
    glVertexArrayVertexBuffer(vaoHandle, 0, this->vbo_vertices, 0, sizeof(float) * 8);

    glVertexArrayAttribFormat(vaoHandle,
        0,				// Attrib ptr
        3,				// Number of data
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
        sizeof(float) * 3);
    glVertexArrayAttribBinding(vaoHandle, 1, 0); // Binding index attri number
    glEnableVertexArrayAttrib(vaoHandle, 1);

    glVertexArrayAttribFormat(vaoHandle,
    2,
    2,
    GL_FLOAT,
    GL_FALSE,
    sizeof(float) * 6);
    glVertexArrayAttribBinding(vaoHandle, 2, 0);
    glEnableVertexArrayAttrib(vaoHandle, 2);

    glNamedBufferData(ibo_elements, sizeof(modelIndices), modelIndices, GL_STATIC_DRAW);
    glVertexArrayElementBuffer(vaoHandle, ibo_elements);

    glBindVertexArray(0);
    std::cout << "Bunny setup done" << std::endl;
}

void Bunny::draw(glm::mat4 projection, glm::mat4 view, ShaderProgram *shader) {
    glUniform3fv(shader->uniform("Material.Ka"), 1, glm::value_ptr(mat.Ka));
    glUniform3fv(shader->uniform("Material.Kd"), 1, glm::value_ptr(mat.Kd));
    glUniform3fv(shader->uniform("Material.Ks"), 1, glm::value_ptr(mat.Ks));
    glUniform1fv(shader->uniform("Material.Shiness"), 1, &mat.Shiness);
    glm::mat4 model = glm::mat4(1.0f);
	model = glm::scale(model, glm::vec3(2.f, 2.f, 2.f));
	glm::mat4 mview = view * model;
	glm::mat4 mvp = projection * view * model;
	glm::mat4 imvp = glm::inverse(model);
	glm::mat3 nmat = glm::mat3(glm::transpose(imvp)); //normal matrix
	glUniformMatrix3fv(shader->uniform("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(nmat));
	glUniformMatrix4fv(shader->uniform("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(shader->uniform("MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
    glBindVertexArray(vaoHandle);
    glDrawElements(GL_TRIANGLES, 432138, GL_UNSIGNED_INT, 0);
}
