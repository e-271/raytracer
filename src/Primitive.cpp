#include "Primitive.h"


///tri v1 v2 v3 create a triangle from 3 vertices
Triangle::Triangle(vec3 vertex1, vec3 vertex2, vec3 vertex3, LightProperties properties, mat4 obj_transforms)
: v1(vertex1), v2(vertex2), v3(vertex3)
{
    this->properties = properties;
    this->obj_transforms = obj_transforms;
    this->normal = glm::normalize(glm::cross((v1-v2), (v3-v2)));
}



bool Triangle::intersects(PrimaryRay& r, Intersection* i){
    ///Does it intersect with the plane?
    //(r.origin + r.direction * t) dot (normal) - (v1) dot (normal) = 0, solved for t
    if(glm::dot(r.direction, normal) == 0) return false;
    float t = (glm::dot(v1, normal) - glm::dot(r.origin, normal)) / (glm::dot(r.direction, normal));
    if(t < 0) return false;

    ///Is it in the triangle?
    //See notes for development of this thing. It's done using parametric equations, barycentric coords, etc.
    ///TODO document this &or develop a better method.
    else {
        float a1 = v1.x, a2 = v1.y, b1 = v2.x, b2 = v2.y, c1 = v3.x, c2 = v3.y, p1 = (r.origin + r.direction*t).x, p2 = (r.origin + r.direction*t).y;
        float denominator = (a2*b1 - a1*b2 - a2*c1 + b2*c1 + a1*c2 - b1*c2);
        float alpha =  (b2*c1 - b1*c2 - b2*p1 + c2*p1 + b1*p2 - c1*p2) / denominator;
        float beta = (-(a2*c1) + a1*c2 + a2*p1 - c2*p1 - a1*p2 + c1*p2)/ denominator;
        float gamma = (a2*b1 - a1*b2 - a2*p1 + b2*p1 + a1*p2 - b1*p2)/ denominator;

        if((alpha >= 0) && (beta >= 0) && (gamma >= 0)){
            i->light = this->properties;
            i->point = r.origin + r.direction * t;
            i->distance = glm::length(i->point - r.origin);
            i->normal = this->normal;

            return true;
            }
        else return false;
    }



}
bool Triangle::intersects(ShadowRay& r){

    ///Does it intersect with the plane?
    //(r.origin + r.direction * t) dot (normal) - (v1) dot (normal) = 0, solved for t
    float t = (glm::dot(v1, normal) - glm::dot(r.origin, normal)) / (glm::dot(r.direction, normal));
    if(t < 0) return false;

    ///Is it in the triangle?
    //See notes for development of this thing. It's done using parametric equations and barycentric coords.
    ///TODO document this &or develop a better method.
    else {
        float a1 = v1.x, a2 = v1.y, b1 = v2.x, b2 = v2.y, c1 = v3.x, c2 = v3.y, p1 = (r.origin + r.direction*t).x, p2 = (r.origin + r.direction*t).y;
        float denominator = (a2*b1 - a1*b2 - a2*c1 + b2*c1 + a1*c2 - b1*c2);
        float alpha =  (b2*c1 - b1*c2 - b2*p1 + c2*p1 + b1*p2 - c1*p2) / denominator;
        float beta = (-(a2*c1) + a1*c2 + a2*p1 - c2*p1 - a1*p2 + c1*p2)/ denominator;
        float gamma = (a2*b1 - a1*b2 - a2*p1 + b2*p1 + a1*p2 - b1*p2)/ denominator;

        if((alpha >= 0) && (beta >= 0) && (gamma >= 0)) return true;
        else return false;
    }
}


Sphere::Sphere(vec3 center, float radius, LightProperties lp, mat4 obj_transforms) {
    this->center = center;
    this->radius = radius;
    this->properties = lp;
    this->obj_transforms = obj_transforms;
}

//Fill in intersect struct if intersect is found, else return false
bool Sphere::intersects(PrimaryRay& r, Intersection* i) {
            //(PdotP) t^2 + 2(Pdot(P0-C)) t + (P0 - C)dot(P0 - C) - r^2 = 0
            //Solving mag((r.origin + r.direction * t) - sphere.center) = r^2 for t

            ///Should I be normalizing slope and stuff? Somewhere I think, maybe in Ray constructors
            vec3 origin_to_center = (r.origin - this->center);
            ///Do I normalize direction? Should I? If so this should just be 1, always.
            float a = glm::dot(r.direction, r.direction);
            float b = 2 * glm::dot(r.direction, origin_to_center);
            float c = glm::dot(origin_to_center, origin_to_center) - (this->radius * this->radius);
            float discriminant = b*b - 4*a*c;

            //complex roots only
            if(discriminant < 0){
                return false;
            }

            //1 root, so we're inside the sphere
            ///so like if r.direction is normalized then a dot a is always gonna be positive 1. this will only give a positive value for r.direction dot origin_to_center < 0
            float closest_x;
            if((0.01f > discriminant) && (discriminant > 0.01f)){
                closest_x = -b / (2 * a);
                if(closest_x < 0) return false;
            }
            //discriminant greater than zero, 2 roots
            else {
                float x1 = (-b + glm::sqrt(discriminant)) / (2 * a);
                float x2 = (-b - glm::sqrt(discriminant)) / (2 * a);
                //use the smallest positive point to get intersect
                //here, maybe, some wiggle room could be put in s.t. near-zero stuff gets discarded
                ///current logic: if your ray starts at the edge of the sphere, or is tangent to the sphere, no intersect.
                if((x1 == x2) /*tangent*/ || (x1 <= 0 && x2 <= 0)){
                    return false;
                }
                else if((x2 < 0) || (0 < x1 && x1 < x2)) {
                    closest_x = x1;
                }
                else if((x1 < 0) || (0 < x2 && x2 < x1)) {        //remove test for speed
                    closest_x = x2;
                }
            }
            i->point = r.origin + r.direction * closest_x;
            i->light = this->properties;
            i->normal = glm::normalize(i->point - this->center);
            i->distance = glm::length(i->point - r.origin);
            return true;
        }

///TODO put in some wiggle room s.t. being slightly under the surface of an object doesn't return an intersect
bool Sphere::intersects(ShadowRay& r) {
            vec3 origin_to_center = (r.origin - this->center);
            float a = glm::dot(r.direction, r.direction);
            float b = 2 * glm::dot(r.direction, origin_to_center);
            float c = glm::dot(origin_to_center, origin_to_center) - (this->radius * this->radius);
            float discriminant = b*b - 4*a*c;

        //need to discard negative values, because that indicates direction going the wrong way
            if((0.01f > discriminant) && (discriminant > -0.01f)) {
                    float x = -b / (2 * a);
                    if(x > 0) return true;
                }
            else if(discriminant > 0) {
                    float x1 = (-b + glm::sqrt(discriminant)) / (2 * a);
                    float x2 = (-b - glm::sqrt(discriminant)) / (2 * a);
                    if((x1 > 0) || (x2 > 0)) return true;
                }
            return false;
}
