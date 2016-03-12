#include "Raytracer.h"
#include "variables.h"
#include <algorithm>

struct closest_intersect {
    int index;
    vec3 point;
    //vec3 normal; //for later lighting calculations
};

vec3 Raytracer::trace(PrimaryRay& ray) {
    int min_distance = -1;
    int intersect_index = -1;
    Intersection* intersect = (Intersection *) malloc(sizeof(Intersection));    ///This is getting remade every call...
    Intersection closest_intersect;

    for(int i=0; i<num_objects; i++) {
        if(objects[i]->intersects(ray, intersect)) {
            if(intersect->distance < min_distance || min_distance == -1) {
                intersect_index = i;
                closest_intersect = *intersect;
                min_distance = closest_intersect.distance;
            }
        }
    }
    vec3 color = closest_intersect.light.ambient + closest_intersect.light.emission;
    free(intersect);
    //no objects in sight
    if(intersect_index == -1) {
        return color;
    }

    //For each light, iterate through all objects to see if something is blocking it. If not, add the light's color.
    for(int i=0; i<num_lights; i++) {
        vec3 ray_direction = glm::normalize((lights[i].is_directional) ? lights[i].pos : (lights[i].pos - closest_intersect.point));
        ShadowRay shadow((closest_intersect.point + ray_direction*0.01f), ray_direction);

        bool shadowed = false;
        for(int j=0; j<num_objects; j++) {
            if(objects[j]->intersects(shadow)) {
                ///TODO more with this, implement shading models
                shadowed = true;
                break;
            }
        }
        if(!shadowed) {
            color += get_lighting(lights[i], closest_intersect);
        }
    }

    /*
    vec3 lightDirection;
    if(lightPosition.w == 0){
        lightDirection = normalize(lightPosition.xyz);
    }
    else {
        vec3 lightDirection = (lightPosition.xyz / lightPosition.w) - position;
        lightDirection = normalize(distance);
    }
    // phong - specular, shiny  component
    // halfVector - A normallized vector that sits halfway between the lightDirection and eyeDirection vectors
    // normalDotHalf - The projection of normal onto halfVec. Measures on a scale from 0 to 1 how close the half vector is to the normal.
    // The closer the viewer is to the angle of reflection, the larger this value will be (as halfVec approaches the normal vector).
    vec3 halfVec = normalize(lightDirection + eyeDirection);
    //When eyeDirection is too far, like when the object isn't facing you, the dot product will be negative - so we just zero it out.
    float normDotHalf = max(dot(normal, halfVec), 0.0);
    //Because normDotHalf is in [0, 1], it's taken to a power to _diminish_ it.
    //normDotHalf ^ shininess determines how shiny it is, specular determines what color the highlight is.
    vec4 phong = specular * lightColorx * pow(normDotHalf, shininess) ;

    // lambert - diffuse component
    float normDotLight = max(dot(lightDirection, normal), 0.0);
    vec4 lambert = diffuse * lightColorx * normDotLight ;

    return phong + lambert;

    */

    ///TODO this
    //if object is reflective
    //    angle = getReflectionAngle
    //    ray = generateRayFromAngle
    //    color += trace(Ray ray)

    return color;
}

vec3 Raytracer::get_lighting(Light light, Intersection intersect) {\
    vec3 eye_to_object = intersect.point;
    vec3 object_to_light = light.is_directional ? light.pos : light.pos - intersect.point;
    vec3 normal = intersect.normal;
    vec3 halfway = eye_to_object + object_to_light;

    halfway = glm::normalize(halfway);
    object_to_light = glm::normalize(object_to_light);
    eye_to_object = glm::normalize(eye_to_object);

    float brightness = std::max( glm::dot(object_to_light, normal) , 0.0f );
    vec3 diffuse = intersect.light.diffuse;
    vec3 lambertian_color = diffuse * light.color * brightness;

    return lambertian_color + intersect.light.ambient;
}


//gets made in camera's constructor
Raytracer::Raytracer(Primitive** objects, int num_objects, Light* lights, int num_lights)
{
    this->objects = objects;
    this->num_objects = num_objects;
    this->lights = lights;
    this->num_lights = num_lights;

}

Raytracer::~Raytracer()
{
    //dtor
}
