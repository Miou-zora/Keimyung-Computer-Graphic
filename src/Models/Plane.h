#pragma once

#include <Windows.h>
#include <GL/gl3w.h>
#include <GL/GL.h>
#include <glm/mat4x4.hpp>

#include <vector>

#include "Loader.h"
#include "Mesh.hpp"



class Plane
{
public:
	Plane(unsigned int nbr_of_square = 40);
	~Plane() = default;

	void draw(glm::mat4 projection, glm::mat4 view, ShaderProgram *shader);

private:
	GLuint nVerts, elements;
	unsigned int number_of_square;
	std::vector<Mesh> meshes;

	void generateMesh();
};
