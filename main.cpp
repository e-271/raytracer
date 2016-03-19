#include <iostream>
#include <Primitive.h>
#include "Scene.h"
#include <FreeImage.h>
#include <string>

using namespace std;

///need to eventually move some public variables into private
///So the stuff that I can do with this:
///make it meet the class' specs, with reading input files and stuff
///implement Phong shading
///do more stuff with light properties
///object transformatins
///triangles

int main()
{
    FreeImage_Initialise();

    vec3 ambient(0,0,0);
    vec3 diffuse(1,1,1);
    vec3 specular(0,0,0);
    vec3 emission(0,0,0);
    int shininess = 1;
    LightProperties properties(diffuse, ambient, specular, emission, shininess);
    //The direction of a directional light corresponds to the direction of any shadow vectors it spawns
    vec3 lightPos = vec3(0, 0, 1);
    vec3 lightColor = vec3(1, 0, 0);
    bool directional = true;
    Light light(lightPos, lightColor, directional);

    mat4 transforms = mat4(1);

    Triangle triangle1 = Triangle(vec3(-4, -1, -6), vec3(0, -2, -2), vec3(4, -1, -6), properties, transforms);
    Triangle triangle2 = Triangle(vec3(0, 1, -2), vec3(-1, 0, -2), vec3(1, 0, -2), properties, transforms);
    Sphere sphere1 = Sphere(vec3(-1,0,-5), 0.5, properties, transforms);
    Sphere sphere2 = Sphere(vec3(1,0,-3), 0.5, properties, transforms);
    Primitive* objects[] = {&sphere1, &sphere2};

    Raytracer raytracer = Raytracer(&objects[0], 2, &light, 1);
    Camera cam(vec3(0,0,0), vec3(0,1,0), vec3(0,0,-1), 90, 90);
    Scene scene(cam, raytracer, 500, 500); //Right now scene only has one function. Maybe remove unless I end up expanding it with picture making.
    scene.render();

    if(!FreeImage_Save(FIF_PNG, scene.bitmap, "img.png", 0))
        std::cout<<"bad things"<<std::endl;
    FreeImage_DeInitialise();

    return 0;

}
