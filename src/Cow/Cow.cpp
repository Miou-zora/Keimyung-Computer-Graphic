// ColorCube.cpp
#include "Cow.h"
#include "cow_data.h"
#include <vector>
#include <GL/gl.h>
#include <cstdio>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

Cow::Cow() {
    setup();
}

Cow::~Cow() {
    glDeleteBuffers(1, &vbo_vertices);
    glDeleteBuffers(1, &vbo_normals);
    glDeleteBuffers(1, &ibo_elements);
    glDeleteVertexArrays(1, &vaoHandle);
}

void Cow::setup() {
    std::vector<glm::vec3> normals_faces;
    normals_faces.resize(9468 / 3); //# of vertices 9468 -> # of normal 9468

    for (int i = 0; i < 9468 / 3; i++) {
        const glm::vec3& v0 = COW::vertices[COW::nvertices[i * 3]]; //1st vertex
        const glm::vec3& v1 = COW::vertices[COW::nvertices[i * 3 + 1]]; //2nd vertex
        const glm::vec3& v2 = COW::vertices[COW::nvertices[i * 3 + 2]]; //3rd vertex
        glm::vec3 n = glm::cross((v1 - v0), (v2 - v0)); //Cross product
        n = glm::normalize(n);
        
        normals_faces[i] = n; //Set the same normal to the face
    }

    std::vector<glm::vec3> normals_vertices;
    normals_vertices.resize(1732);

    for (int i = 0; i < 1732; i++) {
        glm::vec3 sum = glm::vec3(0.0f);
        int count = 0;
        for (int j = 0; j < 9468 / 3; j++) {
            if (COW::nvertices[j * 3] == i || COW::nvertices[j * 3 + 1] == i || COW::nvertices[j * 3 + 2] == i) {
                sum += normals_faces[j];
                count++;
            }
        }
        normals_vertices[i] = sum / (float)count;
    }

    mat.Shiness = 51.2f;
    mat.Ka = glm::vec3(0.24725f, 0.1995f, 0.0745f);
    mat.Kd = glm::vec3(0.75164f, 0.60648f, 0.22648f);
    mat.Ks = glm::vec3(0.628281f, 0.555802f, 0.366065f);

    glGenVertexArrays(1, &vaoHandle);
    glBindVertexArray(vaoHandle);

    // Vertex positions VBO
    glGenBuffers(1, &vbo_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(COW::vertices), COW::vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // Vertex colors VBO
    glGenBuffers(1, &vbo_normals);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_normals);
    glBufferData(GL_ARRAY_BUFFER,  normals_vertices.size() * sizeof(glm::vec3), normals_vertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    // Element indices buffer
    glGenBuffers(1, &ibo_elements);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_elements);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(COW::nvertices), COW::nvertices, GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Cow::draw(glm::mat4 projection, glm::mat4 view, ShaderProgram *shader) {
    glUniform3fv(shader->uniform("Material.Ka"), 1, glm::value_ptr(mat.Ka));
    glUniform3fv(shader->uniform("Material.Kd"), 1, glm::value_ptr(mat.Kd));
    glUniform3fv(shader->uniform("Material.Ks"), 1, glm::value_ptr(mat.Ks));
    glUniform1fv(shader->uniform("Material.Shiness"), 1, &mat.Shiness);
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(6.0f, -1.0f, 0.0f));
	model = glm::scale(model, glm::vec3(0.3f, 0.3f, 0.3f));
	glm::mat4 mview = view * model;
	glm::mat4 mvp = projection * view * model;
	glm::mat4 imvp = glm::inverse(model);
	glm::mat3 nmat = glm::mat3(glm::transpose(imvp)); //normal matrix
	glUniformMatrix3fv(shader->uniform("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(nmat));
	glUniformMatrix4fv(shader->uniform("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(shader->uniform("MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
    glBindVertexArray(vaoHandle);
    glDrawElements(GL_TRIANGLES, 9468, GL_UNSIGNED_INT, 0);
}
