// ColorCube.h
#pragma once

#include <GL/gl3w.h>
#include <GL/gl.h>
#include <glm/glm.hpp>
#include "Loader.h"
#include "Material.hpp"

class Cow {
private:
    GLuint vaoHandle;
    GLuint vbo_vertices, vbo_normals, ibo_elements;

    void setup();

public:
    Cow();
    ~Cow();
    Material mat;
    void draw(glm::mat4 projection, glm::mat4 view, ShaderProgram *shader);
};
