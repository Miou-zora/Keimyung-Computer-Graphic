// ColorCube.h
#pragma once
#include <GL/gl3w.h>

class ColorCube {
private:
    GLuint vaoHandle;
    GLuint vbo_cube_vertices, vbo_cube_colors, ibo_cube_elements;

    void setup();

public:
    ColorCube();
    ~ColorCube();
    void draw();
};
