#ifndef VBOTORUS_H
#define VBOTORUS_H

#include <GL/gl3w.h>
#include <glm/mat4x4.hpp>
#include "Loader.h"
#include "Material.hpp"


class VBOTorus 
{
private:
    GLuint vaoHandle;
    int faces, rings, sides;
	GLuint vbo_cube_vertices, vbo_cube_normals, ibo_cube_elements;
    void generateVerts(float * , float * , unsigned int *,
                       float , float);

public:
    Material mat;
    VBOTorus() : VBOTorus(0.7f, 0.3f, 50, 50) {}
    VBOTorus(float, float, int, int);
	virtual ~VBOTorus();
	void draw(glm::mat4 projection, glm::mat4 view, ShaderProgram *shader) const;
};

#endif // VBOTORUS_H
