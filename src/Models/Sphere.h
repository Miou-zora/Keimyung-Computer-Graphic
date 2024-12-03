#ifndef ___SPHERE_H
#define ___SPHERE_H

#include <Windows.h>
#include <GL/gl3w.h>
#include <GL/GL.h>
#include <glm/mat4x4.hpp>

#include <vector>

#include "Loader.h"
#include "Material.hpp"


class Sphere
{
public:
	Sphere();
	Sphere(float rad, GLuint sl, GLuint st);
	~Sphere();

	void setup();
	void draw(glm::mat4 projection, glm::mat4 view, ShaderProgram *shader);
	int getVertexArrayHandle();

	GLuint VAO, VBO_position, VBO_normal, IBO;

	

private:
	Material mat;
	GLuint nVerts, elements;
	float radius;
	GLuint slices, stacks;

	void generateVerts(float *, float *, float *, GLuint *);


};


#endif
