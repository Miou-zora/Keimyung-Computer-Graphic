#pragma once

#include <GL/gl3w.h>
#include <GL/GL.h>
#include <glm/vec3.hpp>

class Material
{
    public:
        Material(GLfloat shiness = 10.f,
        glm::vec3 ka = glm::vec3(0.2, 0.2, 0.2),
        glm::vec3 kd = glm::vec3(1.f, 1.f, 1.f),
        glm::vec3 ks = glm::vec3(1.f, 1.f, 1.f)): Shiness(shiness), Ka(ka), Kd(kd), Ks(ks) {};

        GLfloat Shiness;
        glm::vec3 Ka;
        glm::vec3 Kd;
        glm::vec3 Ks;
};