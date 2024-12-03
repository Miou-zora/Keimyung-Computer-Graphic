#include "vbotorus.h"


#include <GL/gl.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/matrix_inverse.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>

#include <iostream>

#include <cstdio>
#include <cmath>

#include "Loader.h"

VBOTorus::VBOTorus(float outerRadius, float innerRadius, int nsides, int nrings) :
        rings(nrings), sides(nsides)
{
    faces = sides * rings;
    int nVerts  = sides * (rings+1);   // One extra ring to duplicate first ring

    // Verts
	GLfloat * v = new GLfloat[3 * nVerts];
    // Normals
	GLfloat * n = new GLfloat[3 * nVerts];
    // // Tex coords
    // float * tex = new float[2 * nVerts];
    // Elements
    unsigned int * el = new unsigned int[6 * faces];

    // Generate the vertex data
    generateVerts(v, n, el, outerRadius, innerRadius);

    0.329412, 0.223529, 0.027451, 1.0,
    0.780392, 0.568627, 0.113725, 1.0,
    0.992157, 0.941176, 0.807843, 1.0,
    27.89743616,

    mat.Shiness = 27.89743616f;
    mat.Ka = glm::vec3(0.329412f, 0.223529f, 0.027451f);
    mat.Kd = glm::vec3(0.780392f, 0.568627f, 0.113725f);
    mat.Ks = glm::vec3(0.992157f, 0.941176f, 0.807843f);

    //create vao, vbo and ibo here... (We didn't use std::vector here...)
	glGenVertexArrays(1, &vaoHandle);
    glBindVertexArray(vaoHandle);

    // Vertex positions VBO
    glGenBuffers(1, &vbo_cube_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
    glBufferData(GL_ARRAY_BUFFER, 3 * nVerts * sizeof(GLfloat), v, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // Vertex Normal VBO
    glGenBuffers(1, &vbo_cube_normals);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_normals);
    glBufferData(GL_ARRAY_BUFFER,  3 * nVerts * sizeof(GLfloat), n, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    // Element indices buffer
    glGenBuffers(1, &ibo_cube_elements);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, faces * 6 * sizeof(unsigned int), el, GL_STATIC_DRAW);

    glBindVertexArray(0);
	std::cout << "Torus created" << std::endl;

    delete [] v;
    delete [] n;
    delete [] el;
}


VBOTorus::~VBOTorus()
{
    glDeleteBuffers(1, &vbo_cube_vertices);
    glDeleteBuffers(1, &vbo_cube_normals);
    glDeleteBuffers(1, &ibo_cube_elements);
    glDeleteVertexArrays(1, &vaoHandle);
}

void VBOTorus::draw(glm::mat4 projection, glm::mat4 view, ShaderProgram *shader) const 
{
    glUniform3fv(shader->uniform("Material.Ka"), 1, glm::value_ptr(mat.Ka));
    glUniform3fv(shader->uniform("Material.Kd"), 1, glm::value_ptr(mat.Kd));
    glUniform3fv(shader->uniform("Material.Ks"), 1, glm::value_ptr(mat.Ks));
    glUniform1fv(shader->uniform("Material.Shiness"), 1, &mat.Shiness);
    glm::mat4 model = glm::translate(glm::mat4(1.0f), glm::vec3(6.0f, 0.0f, 0.0f));
	glm::mat4 mview = view * model;
	glm::mat4 mvp = projection * view * model;
	glm::mat4 imvp = glm::inverse(model);
	glm::mat3 nmat = glm::mat3(glm::transpose(imvp)); //normal matrix
	glUniformMatrix3fv(shader->uniform("NormalMatrix"), 1, GL_FALSE, glm::value_ptr(nmat));
	glUniformMatrix4fv(shader->uniform("ModelMatrix"), 1, GL_FALSE, glm::value_ptr(model));
	glUniformMatrix4fv(shader->uniform("MVP"), 1, GL_FALSE, glm::value_ptr(mvp));
    glBindVertexArray(vaoHandle);
	glDrawElements(GL_TRIANGLES, faces * 6 * sizeof(unsigned int), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

void VBOTorus::generateVerts(GLfloat * verts, GLfloat * norms, unsigned int * el,
                             float outerRadius, float innerRadius)
{
	float TWOPI = 2 * glm::pi<float>();

    float ringFactor  = (float)(TWOPI / rings);
    float sideFactor = (float)(TWOPI / sides);
    int idx = 0, tidx = 0;
    for( int ring = 0; ring <= rings; ring++ ) {
        float u = ring * ringFactor;
        float cu = cos(u);
        float su = sin(u);
        for( int side = 0; side < sides; side++ ) {
            float v = side * sideFactor;
            float cv = cos(v);
            float sv = sin(v);
            float r = (outerRadius + innerRadius * cv);
            verts[idx] = r * cu;
            verts[idx + 1] = r * su;
            verts[idx + 2] = innerRadius * sv;
		
            norms[idx] = cv * cu * r;
            norms[idx + 1] = cv * su * r;
            norms[idx + 2] = sv * r;

//             tex[tidx] = (float)(u / TWOPI);
//             tex[tidx+1] = (float)(v / TWOPI);
//            tidx += 2;
            // Normalize
            float len = sqrt( norms[idx] * norms[idx] +
                              norms[idx+1] * norms[idx+1] +
                              norms[idx+2] * norms[idx+2] );
            norms[idx] /= len;
            norms[idx+1] /= len;
            norms[idx+2] /= len;
            idx += 3;
        }
    }

    idx = 0;
    for( int ring = 0; ring < rings; ring++ ) {
        int ringStart = ring * sides;
        int nextRingStart = (ring + 1) * sides;
        for( int side = 0; side < sides; side++ ) {
            int nextSide = (side+1) % sides;
            // The quad
            el[idx] = (ringStart + side);
            el[idx+1] = (nextRingStart + side);
            el[idx+2] = (nextRingStart + nextSide);
            el[idx+3] = ringStart + side;
            el[idx+4] = nextRingStart + nextSide;
            el[idx+5] = (ringStart + nextSide);
            idx += 6;
        }
    }


}


