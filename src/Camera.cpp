#include "Camera.h"

using namespace glm;

Camera::Camera(vec3 eye, vec3 up, vec3 center, int fovx, int fovy)
    : eye(eye), up(up), center(center), fovx(fovx), fovy(fovy)
{
    this->z_axis = -normalize(center - eye);    //!
    this->x_axis = normalize(cross(up, z_axis));
    this->y_axis = normalize(cross(z_axis, x_axis));
    this->tan_fovx = glm::tan( (fovx / 2) * PI / 180);
    this->tan_fovy = glm::tan( (fovy / 2) * PI / 180 );
}

Camera::~Camera()
{
    //dtor
}
