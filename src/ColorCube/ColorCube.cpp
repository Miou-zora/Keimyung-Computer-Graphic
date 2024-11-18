// ColorCube.cpp
#include "ColorCube.h"
#include <vector>

ColorCube::ColorCube() {
    setup();
}

ColorCube::~ColorCube() {
    glDeleteBuffers(1, &vbo_cube_vertices);
    glDeleteBuffers(1, &vbo_cube_colors);
    glDeleteBuffers(1, &ibo_cube_elements);
    glDeleteVertexArrays(1, &vaoHandle);
}

void ColorCube::setup() {
    // Cube vertices
    const GLfloat vertices[] = {
        -0.5, -0.5, -0.5,  0.5, -0.5, -0.5,  0.5,  0.5, -0.5, -0.5,  0.5, -0.5,
        -0.5, -0.5,  0.5,  0.5, -0.5,  0.5,  0.5,  0.5,  0.5, -0.5,  0.5,  0.5
    };

    // Cube colors
    const GLfloat colors[] = {
        1.0, 0.0, 0.0,  1.0, 1.0, 0.0,  0.0, 1.0, 0.0,  0.0, 0.0, 1.0,
        1.0, 0.0, 1.0,  1.0, 1.0, 1.0,  0.0, 1.0, 1.0,  0.0, 0.0, 0.0
    };

    // Cube element indices
    const GLushort elements[] = {
        0, 1, 2,  2, 3, 0,  // Back face
        4, 5, 6,  6, 7, 4,  // Front face
        0, 4, 7,  7, 3, 0,  // Left face
        1, 5, 6,  6, 2, 1,  // Right face
        3, 2, 6,  6, 7, 3,  // Top face
        0, 1, 5,  5, 4, 0   // Bottom face
    };

    glGenVertexArrays(1, &vaoHandle);
    glBindVertexArray(vaoHandle);

    // Vertex positions VBO
    glGenBuffers(1, &vbo_cube_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // Vertex colors VBO
    glGenBuffers(1, &vbo_cube_colors);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_colors);
    glBufferData(GL_ARRAY_BUFFER, sizeof(colors), colors, GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    // Element indices buffer
    glGenBuffers(1, &ibo_cube_elements);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(elements), elements, GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void ColorCube::draw() {
    glBindVertexArray(vaoHandle);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_SHORT, 0);
}
