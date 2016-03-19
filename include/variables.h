#ifndef VARIABLES_H
#define VARIABLES_H
//TODO import GLM
#include "glm/glm.hpp"
#include "gtc/matrix_transform.hpp"

typedef glm::vec3 vec3;
typedef glm::vec4 vec4;
typedef glm::mat3 mat3;
typedef glm::mat4 mat4;
typedef unsigned int color[3];

#ifndef PI
#define PI 3.14159265359
#endif // PI

///consider creating a Ray class with these 2 things as children, and perhaps the intersection functions as members
///if these guys have
struct LightProperties {
    vec3 diffuse;
    vec3 ambient;
    vec3 specular;
    vec3 emission;
    int shininess;

LightProperties() {}
LightProperties(vec3 diff, vec3 amb, vec3 spec, vec3 emmis, int shiny) {
    diffuse = diff;
    ambient = amb;
    specular = spec;
    emission = emmis;
    shininess = shiny;
}
};

struct Intersection {
    int distance;
    vec3 point;
    vec3 normal;
    LightProperties light;
};

struct Light {
    vec3 pos;
    vec3 color;
    bool is_directional = false;
    vec3 attenuation; // default (1,0,0);        //constant, linear, and quadratic attenuation
    vec3 ambient; // default(0.2, 0.2, 0.2);    //global ambient color to be added for each object

    Light() {}
    Light(vec3 posn, vec3 colr, bool isdirectional) {
    pos = posn;
    color = colr;
    is_directional = isdirectional;
    }
};

typedef vec4 quaternion;

#endif // VARIABLES_H


