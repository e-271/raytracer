#include "Scene.h"
#include "Raytracer.h"
#include <cstring>
#include <iostream>

///I think I want to integrate Camera and Scene together. Camera doesn't (currently, keep that in mind) do much except pass off variables to Scene.

Scene::Scene(Camera& camera, Raytracer& raytracer, int img_height, int img_width)
    : camera(camera), raytracer(raytracer), img_height(img_height), img_width(img_width)
{
    //3 bytes per pixel.
    bitmap = FreeImage_Allocate(img_width, img_height, 24);
}

Scene::~Scene()
{

}

void Scene::render() {

    for(h_counter = 0; h_counter<img_height; h_counter++){
        for(w_counter = 0; w_counter<img_width; w_counter++){
            vec3 color = vec3(0,0,0);
            PrimaryRay ray = PrimaryRay(w_counter, h_counter, camera, img_width, img_height);
            color = raytracer.trace(ray);
            record_color(color);

            if(color[0] > 0 || color[1] > 0 || color[2] > 0) std::cout << ".";
            else std::cout << " ";
        }
        std::cout<<std::endl;
    }
}


//Needs: an image object to write to, and a pointer to the spot being writ to, and it need to convert all the stuff to bytes (char)
void Scene::record_color(vec3 color) {
    ///we'll see if this works
    RGBQUAD pixel_color;
    pixel_color.rgbRed = 255 * color.x;
    pixel_color.rgbBlue = 255 * color.y;
    pixel_color.rgbGreen = 255 * color.z;
    FreeImage_SetPixelColor(bitmap,w_counter,img_height - h_counter,&pixel_color);

    //I guess output is 3 bytes per pixel?


}
