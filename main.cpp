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

int main()
{
    ///TODO get phong shading working and do reflections
    FreeImage_Initialise();

    //The direction of a directional light corresponds to the direction of any shadow vectors it spawns
    vec3 lightPos = vec3(0, 1, 0);
    vec3 lightColor = vec3(1, 0, 0);
    bool directional = false;
    Light light1(lightPos, lightColor, directional);
    lightPos = vec3(1, 1, 0);
    lightColor = vec3(0,1,0);
    directional = false;
    Light light2(lightPos, lightColor, directional);
    Light* lights[] = {&light1, &light2};

    mat4 transforms = mat4(1);
    ///I need a better way of doing these properties, this is sort of ugly. Maybe add some setters to the Primitive class.

    //Light properties for the spheres
    vec3 ambient(0.01,0.01,0.01);
    vec3 diffuse(0.75,1,1);
    vec3 specular(0.5,0.5,1);
    vec3 emission(0,0,0);
    int shininess = 100;
    LightProperties properties(diffuse, ambient, specular, emission, shininess);
    Sphere sphere1 = Sphere(vec3(-1,0,-4), 0.5, properties, transforms);
    Sphere sphere2 = Sphere(vec3(1,0,-3), 0.5, properties, transforms);

    //Properties for the triangles
    specular = vec3(0,0,0);
    shininess = 0;
    properties = LightProperties(diffuse, ambient, specular, emission, shininess);
    Triangle triangle1 = Triangle(vec3(-400, -1, -200), vec3(0, -2, -2), vec3(400, -1, -200), properties, transforms);
    Triangle triangle2 = Triangle(vec3(0, 1, -2), vec3(-1, 0, -2), vec3(1, 0, -2), properties, transforms);

    Primitive* objects[] = {&triangle1, &sphere1, &sphere2};

    Raytracer raytracer = Raytracer(&objects[0], 3, &lights[0], 2);
    Camera cam(vec3(0,0,0), vec3(0,1,0), vec3(0,0,-1), 90, 90);
    Scene scene(cam, raytracer, 500, 500); //Right now scene only has one function. Maybe remove unless I end up expanding it with picture making.
    scene.render();

    if(!FreeImage_Save(FIF_PNG, scene.bitmap, "img.png", 0))
        std::cout<<"bad things"<<std::endl;
    FreeImage_DeInitialise();

    return 0;

}
