#ifndef LENGTHCONSTRAINT_H
#define LENGTHCONSTRAINT_H
#include <glm/vec3.hpp>

struct LengthConstraint
{
    unsigned int v0 = 0;
    unsigned int v1 = 0;
    float initial_length = 0.0f;
};

float lengthConstraint(glm::vec3 p1, glm::vec3 p2, float length);
glm::vec3 lengthConstraintGradient(glm::vec3 p1, glm::vec3 p2);
#endif // LENGTHCONSTRAINT_H
