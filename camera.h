#ifndef CAMERA_H
#define CAMERA_H
#include <glm/glm.hpp>

class Camera
{
public:
    Camera();
    Camera(glm::vec3 pos, glm::vec3 up, glm::vec3 dir);

    void move(glm::vec3 offset);
    glm::mat4& get_view_mat();
    glm::vec3 getPosition();
private:
    glm::mat4 view_mat;

    glm::vec3 position = glm::vec3(0.0f,0.0f,0.0f);
    glm::vec3 up = glm::vec3(0.0f,1.0f,0.0f);
    glm::vec3 dir = glm::vec3(0.0f,0.0f,-1.0f);
};

#endif // CAMERA_H
