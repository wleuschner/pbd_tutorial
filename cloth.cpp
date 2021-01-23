#include "cloth.h"

Cloth::Cloth()
{

}

Cloth::Cloth(int width, int height)
{
    grid_size_x = width;
    grid_size_y = height;
    vertices = Buffer(Buffer::VERTEX_BUFFER);
    indices = Buffer(Buffer::INDEX_BUFFER);
    //Create Vertex Buffer
    verts.resize(width*height);
    attributes.resize(width*height);
    for(int y=0;y<height;y++)
    {
        float dy = -y+(height/2.0);
        for(int x=0;x<width;x++)
        {
            float dx = -x+(width/2.0);
            verts[y*(width)+x].pos = glm::vec3(dx,dy,0.0f);
            verts[y*(width)+x].normal = glm::vec3(0.0f,0.0f,1.0f);
            attributes[y*(width)+x].vel = glm::vec3(0.0f,0.0f,0.0f);
            attributes[y*(width)+x].mass = y > 0 ? 1.0f : 0.0f;
        }
    }

    //Create Index Buffer
    unsigned int num_patches = (width-1)*(height-1);
    unsigned int num_tris = num_patches*2;
    unsigned int num_idx = num_tris*3;
    std::vector<unsigned int> idx;
    idx.resize(num_idx);
    unsigned int* idx_ptr = idx.data();
    for(int y=0;y<height-1;y++)
    {
        for(int x=0;x<width-1;x++)
        {
            unsigned int v1 = y*width+x;
            unsigned int v2 = (y+1)*width+x;
            unsigned int v3 = (y+1)*width+(x+1);
            unsigned int v4 = y*width+(x+1);

            *idx_ptr = v1;
            idx_ptr++;
            *idx_ptr = v2;
            idx_ptr++;
            *idx_ptr = v4;
            idx_ptr++;

            *idx_ptr = v4;
            idx_ptr++;
            *idx_ptr = v2;
            idx_ptr++;
            *idx_ptr = v3;
            idx_ptr++;
        }
    }
    indices.uploadData((unsigned char*)idx.data(),sizeof(unsigned int)*idx.size());

    calculateInitialLengths(width,height);
}

void Cloth::calculateInitialLengths(int width,int height)
{

    //Add vertical Constraints
    for(int x=0;x<width;x++)
    {
        for(int y=0;y<height-1;y++)
        {
            LengthConstraint constraint;
            constraint.v0 = y*width+x;
            constraint.v1 = (y+1)*width+x;
            constraint.initial_length = glm::length(verts[constraint.v0].pos-verts[constraint.v1].pos);
            length_constraints.push_back(constraint);
        }
    }

    //Add horizontal Constraints
    for(int y=1;y<height;y++)
    {
        for(int x=0;x<width-1;x++)
        {
            LengthConstraint constraint;
            constraint.v0 = y*width+x;
            constraint.v1 = y*width+(x+1);
            constraint.initial_length = glm::length(verts[constraint.v0].pos-verts[constraint.v1].pos);
            length_constraints.push_back(constraint);
        }
    }
}

void Cloth::solve(float timestep, int iterations, std::vector<SphereCollisionConstraint>& spheres)
{
    //Apply external forces
    for(int p=0;p<attributes.size();p++)
    {
        attributes[p].vel += timestep * attributes[p].mass * glm::vec3(0.0,-9.81,0.0);
    }

    //Initial projected positions based on velocity
    for(int p=0;p<attributes.size();p++)
    {
        attributes[p].proj_pos = verts[p].pos + timestep * attributes[p].vel;
    }

    //Add colosion Constraints if any
    sphere_collision_constraints.clear();

    //Solver Constraints and update projected positions
    for(int i=0;i<iterations;i++)
    {
        for(int c=0;c<length_constraints.size();c++)
        {
            glm::vec3 p0 = attributes[length_constraints[c].v0].proj_pos;
            glm::vec3 p1 = attributes[length_constraints[c].v1].proj_pos;
            float initial_length = length_constraints[c].initial_length;
            float m0 = attributes[length_constraints[c].v0].mass;
            float m1 = attributes[length_constraints[c].v1].mass;

            float len = lengthConstraint(p0,p1,initial_length);
            glm::vec3 g0 = lengthConstraintGradient(p0,p1);
            glm::vec3 g1 = lengthConstraintGradient(p1,p0);

            float s0 = len/(m0*glm::dot(g0,g0)+m1*glm::dot(g1,g1));

            attributes[length_constraints[c].v0].proj_pos += -s0*m0*g0;
            attributes[length_constraints[c].v1].proj_pos += -s0*m1*g1;
        }
        for(int p=0;p<attributes.size();p++)
        {
            glm::vec3 p0 = attributes[p].proj_pos;
            for(int c=0;c<spheres.size();c++)
            {
                SphereCollisionConstraint scp = spheres[c];
                float len = sphereCollisionConstraint(p0,scp.center,scp.radius);
                if(len>0.0) continue;
                glm::vec3 g0 = sphereCollisionConstraintGradient(p0,scp.center);
                float s0 = len/(glm::dot(g0,g0));

                attributes[p].proj_pos += -s0*g0;
            }
        }
    }

    //Update velocities and positions
    for(int p=0;p<attributes.size();p++)
    {
        attributes[p].vel = (attributes[p].proj_pos - verts[p].pos) / timestep;
        verts[p].pos = attributes[p].proj_pos;
    }
    recalculateNormals();
}

void Cloth::recalculateNormals()
{
    for(int i=0;i<verts.size();i++)
    {
        verts[i].normal = glm::vec3(0.0f);
    }

    for(int y=0;y<grid_size_y-1;y++)
    {
        for(int x=0;x<grid_size_x-1;x++)
        {
            unsigned int id1 = y*grid_size_x+x;
            unsigned int id2 = (y+1)*grid_size_x+x;
            unsigned int id3 = (y+1)*grid_size_x+(x+1);
            unsigned int id4 = y*grid_size_x+(x+1);

            glm::vec3 v1 = verts[id1].pos;
            glm::vec3 v2 = verts[id2].pos;
            glm::vec3 v3 = verts[id3].pos;
            glm::vec3 v4 = verts[id4].pos;

            {
                glm::vec3 a = v2-v1;
                glm::vec3 b = v4-v1;
                glm::vec3 per_face_normal = glm::normalize(glm::cross(a,b));
                verts[id1].normal += per_face_normal;
                verts[id2].normal += per_face_normal;
                verts[id4].normal += per_face_normal;
            }

            {
                glm::vec3 a = v4-v2;
                glm::vec3 b = v3-v2;
                glm::vec3 per_face_normal = glm::normalize(glm::cross(a,b));
                verts[id4].normal += per_face_normal;
                verts[id2].normal += per_face_normal;
                verts[id3].normal += per_face_normal;
            }
        }
    }

    for(int i=0;i<verts.size();i++)
    {
        verts[i].normal = glm::normalize(verts[i].normal);
    }
}

Buffer& Cloth::getVertexBuffer()
{
    vertices.uploadData((unsigned char*)verts.data(),sizeof(Vertex)*verts.size());
    return vertices;
}

Buffer& Cloth::getIndexBuffer()
{
    return indices;
}
