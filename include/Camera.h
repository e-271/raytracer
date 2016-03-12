#ifndef CAMERA_H
#define CAMERA_H

#include "variables.h"

//Later I will need to rotate and translate this camera, I think?
//Or can it always be at worldview origin, and objects have worldview coords modified? That may be it.
//I do not think we ever put things in camera coordinates. I think we take imgplane coords and convert them to world coord rays.
class Camera
{
    public:
        //do I need these?
        vec3 eye;
        vec3 up;
        vec3 center;
        int fovx;
        int fovy;

        //for efficient ray creation
        float tan_fovx;
        float tan_fovy;
        vec3 x_axis;
        vec3 y_axis;
        vec3 z_axis;

        Camera(vec3, vec3, vec3, int, int);
        virtual ~Camera();
    protected:
    private:
};

#endif // CAMERA_H
