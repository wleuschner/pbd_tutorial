#ifndef SPHERECOLLISIONCONSTRAINT_H
#define SPHERECOLLISIONCONSTRAINT_H
#include <glm/glm.hpp>

struct SphereCollisionConstraint
{
    glm::vec3 center;
    float radius;
};

float sphereCollisionConstraint(glm::vec3 p1, glm::vec3 center, float radius);

glm::vec3 sphereCollisionConstraintGradient(glm::vec3 p1, glm::vec3 center);

#endif // SPHERECOLLISIONCONSTRAINT_H
