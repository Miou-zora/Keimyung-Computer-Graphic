// ColorCube.h
#pragma once
#include <GL/gl3w.h>

class Cow {
private:
    GLuint vaoHandle;
    GLuint vbo_vertices, vbo_normals, ibo_elements;

    void setup();

public:
    Cow();
    ~Cow();
    void draw();
};
