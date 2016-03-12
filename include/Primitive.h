#ifndef PRIMITIVE_H
#define PRIMITIVE_H

#include "variables.h"
#include "Ray.h"

///consider rename
class Primitive
{
    public:
        mat4 obj_transforms;
        LightProperties properties;
        virtual bool intersects(PrimaryRay&, Intersection*) =0;
        virtual bool intersects(ShadowRay&) =0;
};

class Sphere : public Primitive
{
    public:
        float radius;
        vec3 center;        //This value may need to be modified by modelview
        //consider changing this to accept a bunch of properties then create the struct
        Sphere(vec3, float, LightProperties, mat4);
        //Pass by reference (PrimaryRay&) doesn't copy the object, but it also doesn't treat it like a pointer (INtersection*)
        bool intersects(PrimaryRay&, Intersection*);
        bool intersects(ShadowRay&);        //may not need a pass by reference, but it does save time

};

class Triangle : public Primitive
{
    public:
        vec3 v1, v2, v3;
        vec3 normal;
        Triangle(vec3, vec3, vec3, LightProperties, mat4);
        bool intersects(PrimaryRay&, Intersection*);
        bool intersects(ShadowRay&);
};

#endif // PRIMITIVE_H
