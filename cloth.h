#ifndef CLOTH_H
#define CLOTH_H
#include "buffer.h"
#include "vertex.h"
#include "lengthconstraint.h"
#include "spherecollisionconstraint.h"
#include <vector>
#include <glm/glm.hpp>

class Cloth
{
public:
    struct Particle
    {
        glm::vec3 proj_pos;
        glm::vec3 vel;
        float mass;
    };

    Cloth();
    Cloth(int width, int height);

    void solve(float timestep, int iterations,std::vector<SphereCollisionConstraint>& spheres);
    Buffer& getVertexBuffer();
    Buffer& getIndexBuffer();
private:
    unsigned int grid_size_x;
    unsigned int grid_size_y;

    void calculateInitialLengths(int width, int height);
    void recalculateNormals();

    Buffer vertices;
    Buffer indices;

    std::vector<Vertex> verts;
    std::vector<Particle> attributes;
    std::vector<LengthConstraint> length_constraints;
    std::vector<SphereCollisionConstraint> sphere_collision_constraints;
};

#endif // CLOTH_H
