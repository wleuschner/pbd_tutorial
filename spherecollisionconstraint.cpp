#include "spherecollisionconstraint.h"

float sphereCollisionConstraint(glm::vec3 p1, glm::vec3 center, float radius)
{
    return glm::length(center-p1) - radius;
}

glm::vec3 sphereCollisionConstraintGradient(glm::vec3 p1, glm::vec3 center)
{
    return (1.0f/glm::length(p1-center))*(p1-center);
}
