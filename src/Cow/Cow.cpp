// ColorCube.cpp
#include "Cow.h"
#include "cow_data.h"
#include <vector>

Cow::Cow() {
    setup();
}

Cow::~Cow() {
    glDeleteBuffers(1, &vbo_cube_vertices);
    glDeleteBuffers(1, &vbo_cube_colors);
    glDeleteBuffers(1, &ibo_cube_elements);
    glDeleteVertexArrays(1, &vaoHandle);
}

void Cow::setup() {
    std::vector<glm::vec3> normalsOfFaces;
    normalsOfFaces.resize(9468 / 3); //# of vertices 9468 -> # of normal 9468

    for (int i = 0; i < 9468 / 3; i++) {
        const glm::vec3& v0 = COW::vertices[COW::nvertices[i * 3]]; //1st vertex
        const glm::vec3& v1 = COW::vertices[COW::nvertices[i * 3 + 1]]; //2nd vertex
        const glm::vec3& v2 = COW::vertices[COW::nvertices[i * 3 + 2]]; //3rd vertex
        glm::vec3 n = glm::cross((v1 - v0), (v2 - v0)); //Cross product
        n = glm::normalize(n);
        
        normalsOfFaces[i] = n; //Set the same normal to the face
    }

    std::vector<glm::vec3> normalsOfVertices;
    normalsOfVertices.resize(1732);

    for (int i = 0; i < 1732; i++) {
        glm::vec3 sum = glm::vec3(0.0f);
        int count = 0;
        for (int j = 0; j < 9468 / 3; j++) {
            if (COW::nvertices[j * 3] == i || COW::nvertices[j * 3 + 1] == i || COW::nvertices[j * 3 + 2] == i) {
                sum += normalsOfFaces[j];
                count++;
            }
        }
        normalsOfVertices[i] = sum / (float)count;
    }

    glGenVertexArrays(1, &vaoHandle);
    glBindVertexArray(vaoHandle);

    // Vertex positions VBO
    glGenBuffers(1, &vbo_cube_vertices);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_vertices);
    glBufferData(GL_ARRAY_BUFFER, sizeof(COW::vertices), COW::vertices, GL_STATIC_DRAW);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // Vertex colors VBO
    glGenBuffers(1, &vbo_cube_colors);
    glBindBuffer(GL_ARRAY_BUFFER, vbo_cube_colors);
    glBufferData(GL_ARRAY_BUFFER,  normalsOfVertices.size() * sizeof(glm::vec3), normalsOfVertices.data(), GL_STATIC_DRAW);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    // Element indices buffer
    glGenBuffers(1, &ibo_cube_elements);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo_cube_elements);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(COW::nvertices), COW::nvertices, GL_STATIC_DRAW);

    glBindVertexArray(0);
}

void Cow::draw() {
    glBindVertexArray(vaoHandle);
    glDrawElements(GL_TRIANGLES, 9468, GL_UNSIGNED_INT, 0);
}
