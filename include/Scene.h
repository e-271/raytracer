#ifndef SCENE_H
#define SCENE_H

#include "variables.h"
#include "Camera.h"
#include "Primitive.h"
#include "Raytracer.h"
#include <FreeImage.h>

class Scene
{
    public:
        Raytracer raytracer;
        Camera camera;
        Primitive* objects;
        int num_objects;
        int img_height;
        int img_width;
        FIBITMAP* bitmap;
        int h_counter;
        int w_counter;

        void render();
        void addPrimitive(Primitive&);
        Scene(Camera&, Raytracer&, int, int);
        ~Scene();

    private:
        void record_color(vec3 color);
};

#endif // SCENE_H
