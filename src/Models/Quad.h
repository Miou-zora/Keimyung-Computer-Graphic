#pragma once

#include <Windows.h>
#include <GL/gl3w.h>
#include <GL/GL.h>
#include <glm/mat4x4.hpp>

#include <vector>

#include "Loader.h"
#include "Mesh.hpp"



class Quad
{
public:
	Quad();
	~Quad() = default;

	void draw(glm::mat4 projection, glm::mat4 view, ShaderProgram *shader);

private:
	Mesh mesh = Mesh();
	void generateMesh();
};
