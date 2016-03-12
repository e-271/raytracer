#ifndef RAYTRACER_H
#define RAYTRACER_H

#include "Primitive.h"
#include "variables.h"

class Raytracer
{
    public:
        Primitive** objects;
        int num_objects;
        Light* lights;
        int num_lights;
        //Image image;
        Raytracer(Primitive**, int, Light*, int);
        virtual ~Raytracer();
        vec3 trace(PrimaryRay&);
        vec3 get_lighting(Light, Intersection);
    protected:
    private:
};

#endif // RAYTRACER_H
