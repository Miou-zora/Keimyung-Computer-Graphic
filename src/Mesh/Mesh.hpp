#pragma once

#include <Windows.h>
#include <GL/gl3w.h>
#include <GL/GL.h>
#include <glm/mat4x4.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>
#include <array>

#include "Loader.h"
#include "Material.hpp"

class Mesh {
    public:
        Mesh() = default;
        ~Mesh()
        {
            glDeleteBuffers(1, &VBO_position);
            glDeleteBuffers(1, &VBO_normal);
            glDeleteBuffers(1, &IBO);
            glDeleteVertexArrays(1, &VAO);
        }

        ShaderProgram *shader;
        Material mat = Material();
        std::vector<glm::vec3> vertices;
        std::vector<glm::vec3> normals;
        std::vector<glm::vec<3, unsigned int>> triIndices;

        void draw()
        {
            glUniform3fv(shader->uniform("Material.Ka"), 1, glm::value_ptr(mat.Ka));
            glUniform3fv(shader->uniform("Material.Kd"), 1, glm::value_ptr(mat.Kd));
            glUniform3fv(shader->uniform("Material.Ks"), 1, glm::value_ptr(mat.Ks));
            glUniform1fv(shader->uniform("Material.Shiness"), 1, &mat.Shiness);

            glBindVertexArray(VAO);
            glDrawElements(GL_TRIANGLES, triIndices.size()  * sizeof(glm::vec<3, unsigned int>), GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);
        }

        void generateGlBuffers() 
        {
            //create vao, vbo and ibo here... (We didn't use std::vector here...)
            glGenVertexArrays(1, &VAO);
            glBindVertexArray(VAO);

            // Vertex positions VBO
            glGenBuffers(1, &VBO_position);
            glBindBuffer(GL_ARRAY_BUFFER, VBO_position);
            glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(glm::vec3), vertices.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(0);

            // Vertex Normal VBO
            glGenBuffers(1, &VBO_normal);
            glBindBuffer(GL_ARRAY_BUFFER, VBO_normal);
            glBufferData(GL_ARRAY_BUFFER,  normals.size() * sizeof(glm::vec3), normals.data(), GL_STATIC_DRAW);
            glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
            glEnableVertexAttribArray(1);

            // Element indices buffer
            glGenBuffers(1, &IBO);
            glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
            glBufferData(GL_ELEMENT_ARRAY_BUFFER, triIndices.size() * sizeof(glm::vec<3, unsigned int>), triIndices.data(), GL_STATIC_DRAW);

            glBindVertexArray(0);
        }
    private:
        GLuint VAO, VBO_position, VBO_normal, IBO;
};