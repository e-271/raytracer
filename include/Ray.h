#ifndef RAY_H
#define RAY_H

#include "Camera.h"

class Ray
{
    public:
        vec3 origin;
        vec3 direction;
};

class PrimaryRay : public Ray
{
    public:
        vec3 intersect_pos;
        vec3 normal;
        LightProperties properties;

        PrimaryRay(int, int, Camera&, int, int);
        PrimaryRay(vec3, vec3);
        ~PrimaryRay();

};

class ShadowRay : public Ray
{
    public:
        ShadowRay(vec3, vec3);
        ~ShadowRay();
};




#endif // RAY_H
