#include "lengthconstraint.h"
#include <glm/glm.hpp>

float lengthConstraint(glm::vec3 p1, glm::vec3 p2, float length)
{
    return glm::length(p1-p2)-length;
}

glm::vec3 lengthConstraintGradient(glm::vec3 p1, glm::vec3 p2)
{
    return glm::normalize(p1-p2);
}
