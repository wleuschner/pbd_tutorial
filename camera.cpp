#include "camera.h"
#include <glm/gtc/matrix_transform.hpp>


Camera::Camera()
{
    view_mat = glm::lookAt(position,position+dir,up);
}

Camera::Camera(glm::vec3 pos, glm::vec3 up, glm::vec3 dir)
    : position(pos)
    , up(up)
    , dir(dir)
{
    view_mat = glm::lookAt(position,position+dir,up);
}

void Camera::move(glm::vec3 offset)
{
    glm::vec3 strafe_vec = glm::cross(dir,up);
    position += offset.x * strafe_vec;
    position += offset.y * up;
    position += offset.z * dir;
}

glm::vec3 Camera::getPosition()
{
    return position;
}

glm::mat4& Camera::get_view_mat()
{
    view_mat = glm::lookAt(position,position+dir,up);
    return view_mat;
}
