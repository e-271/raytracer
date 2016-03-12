#include "Ray.h"

ShadowRay::ShadowRay(vec3 origin, vec3 direction)
{
    this->origin = origin;
    this->direction = glm::normalize(direction);
}

ShadowRay::~ShadowRay()
{
    //dtor
}

//There are a whole bunch of variables that this thing doesn't initialize. Not sure what junk indicative values to set them to at this point.
PrimaryRay::PrimaryRay(vec3 origin, vec3 direction)
{
    this->origin = origin;
    this->direction = glm::normalize(direction);
}

//i corresponds to width, j to height
//AS IS TRADITIONAL
PrimaryRay::PrimaryRay(int index_i, int index_j, Camera& camera, int img_width, int img_height) {
    float xfactor = camera.tan_fovx * (index_i - img_width/2.0) / (img_width/2.0);
    float yfactor = camera.tan_fovy * (img_height/2.0 - index_j) / (img_height/2.0);

    this->direction = glm::normalize(-camera.z_axis + (xfactor*camera.x_axis) + (yfactor*camera.y_axis));
    this->origin = camera.eye;
}

PrimaryRay::~PrimaryRay()
{

}




