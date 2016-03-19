#include "Primitive.h"
#define EPSILON 0.01

///So I might not need an intersects method for each subclass, as long as each subclass has its own unique calculate_intersection method.
///Could maybe just do Primitive.intersects. I'd have to implement a get_normal method for subclasses.

///tri v1 v2 v3 create a triangle from 3 vertices
Triangle::Triangle(vec3 vertex1, vec3 vertex2, vec3 vertex3, LightProperties properties, mat4 obj_transforms)
: v1(vertex1), v2(vertex2), v3(vertex3)
{
    this->properties = properties;
    this->obj_transforms = obj_transforms;
    this->normal = glm::normalize(glm::cross((v3 - v2), (v1 - v2)));
}

/*
 Solution found by setting the ray's parametric equation equal to the barycentric coordinate equation for the triangle,
 setting up a new set of parametric equations, and solving for t, beta, gamma.
 Doesn't require you to do ray-plane intersect and therefore doesn't need the triangle's normal.
    To understand what's going on here check this out (pdf also found in the root directory of this project):
    http://www.cs.virginia.edu/~gfx/Courses/2003/ImageSynthesis/papers/Acceleration/Fast%20MinimumStorage%20RayTriangle%20Intersection.pdf
*/
float Triangle::calculate_intersection2(Ray& r) {
    //Transform vectors by -v1 to put v1 at origin
    vec3 t1, t2, t3;
    t2 = this->v2 - this->v1;
    t3 = this->v3 - this->v1;
    vec3 r_origin, r_direction;
    r_origin = r.origin - this->v1;
    r_direction = -r.direction;  //moved to the other side of the equation so we can set up parametric equations
    // mat{r_direction, t2, t3} * vec[t , beta, gamma] = vec[r_origin]
    //For vectors A, B, C, det({A, B, C}) = -(A x C) * B
    float determinant = -glm::dot(glm::cross(r_direction, t3) , t2);
    //Zero determinant means no solution
    //!We seem to need a negative determinant. Not sure of significance of positive determinant.
    if((determinant < EPSILON) && (determinant > -EPSILON)) return -1;
    //Cramer's Rule
    float t = -glm::dot(glm::cross(r_origin, t3), t2) / determinant;
    if(t<0) return -1; //t is the coefficient for direction, so if it's negative the intersection is behind you.
    //Barycentric coords: alpha + beta + gamma = 1
    //alpha, beta, gamma > 0
    float beta = -glm::dot(glm::cross(r_direction, t3), r_origin) / determinant;
    if(beta > 1 || beta < 0) return -1;
    float gamma = -glm::dot(glm::cross(r_direction, r_origin), t2) / determinant;
    if(gamma > 1 || gamma < 0) return -1;
    if(beta + gamma > 1) return -1;
    else return t;
}

//!This is giving me a cleaner result. Maybe glm's calculations are more lossy.
/*
 Solution using ray - plane intersect to get the point t where r.origin + r.direction*t lies within the plane of the triangle.
 alpha, beta, and gamma refer to barycentric coordinate constants,
The equations for them are taken from the solution of a 3x3 matrix whose rows are derived from setting this point:
    P = r.origin + r.direction * t
equal to the equation for barycentric coordinates in terms of the vertices v1, v2, v3 of this triangle:
    P = v1 * alpha + v2 * beta + v3 * gamma
and then used this property of barycentric coordinates:
    beta + alpha + gamma = 1
    therefore v1 = alpha * v1 + beta * v1 + gamma * v1 (that is, 1 * v1 = v1)
subtracting the above form of v1 from both sides to get the system of equations:
    P - v1 = beta (v2 - v1) + gamma(v3 - v1)
    P - v2 = alpha (v1 - v2) + gamma(v3 - v2)
    P - v3 = alpha (v1 - v3) + beta (v2 - v3)
I used 3 of the like 9 parametric equations that come out of this as rows of a 3x3 matrix, plugged it into Wolfram Alpha, and below is the result.
(tl;dr: magic)
*/
float Triangle::calculate_intersection(Ray& r) {
    if(glm::dot(r.direction, normal) == 0) return -1;
    float t = (glm::dot(v1, normal) - glm::dot(r.origin, normal)) / (glm::dot(r.direction, normal));
    if(t < 0) return -1;

    else {
        float a1 = v1.x, a2 = v1.y, b1 = v2.x, b2 = v2.y, c1 = v3.x, c2 = v3.y, p1 = (r.origin + r.direction*t).x, p2 = (r.origin + r.direction*t).y;
        //Only one computation of denominator necessary for each triangle, can store in memory.
        float denominator = (a2*b1 - a1*b2 - a2*c1 + b2*c1 + a1*c2 - b1*c2);
        float alpha =  (b2*c1 - b1*c2 - b2*p1 + c2*p1 + b1*p2 - c1*p2) / denominator;
        float beta = (-(a2*c1) + a1*c2 + a2*p1 - c2*p1 - a1*p2 + c1*p2)/ denominator;
        float gamma = (a2*b1 - a1*b2 - a2*p1 + b2*p1 + a1*p2 - b1*p2)/ denominator;

        if((alpha >= 0) && (beta >= 0) && (gamma >= 0)){
            return t;
            }
        else return -1;
    }
}

//May need some epsilon or some jumping up of t to handle rounding errors
bool Triangle::intersects(PrimaryRay& r, Intersection* i){
    float t = calculate_intersection(r);

    if(t < -EPSILON) return false;
    if((t > -EPSILON) && (t < 0)) t = 0;    //round up negative values of t within epsilon range to zero
    else {
        i->light = this->properties;
        i->point = r.origin + r.direction * t;
        i->distance = glm::length(i->point - r.origin);
        i->normal = this->normal;
        return true;
    }
}


bool Triangle::intersects(ShadowRay& r){
    if(calculate_intersection(r) < -EPSILON) return false;
    else return true;
}


Sphere::Sphere(vec3 center, float radius, LightProperties lp, mat4 obj_transforms) {
    this->center = center;
    this->radius = radius;
    this->properties = lp;
    this->obj_transforms = obj_transforms;
}

float Sphere::calculate_intersection(Ray& r) {
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
            if(discriminant < 0) return -1;
            float closest_x;
            //1 root, so we're inside the sphere
            if((0.01f > discriminant) && (discriminant > 0.01f)){
                closest_x = -b / (2 * a);
                if(closest_x < 0) return -1;
            }
            //discriminant greater than zero, 2 roots
            else {
                float x1 = (-b + glm::sqrt(discriminant)) / (2 * a);
                float x2 = (-b - glm::sqrt(discriminant)) / (2 * a);
                //use the smallest positive point to get intersect
                //here, maybe, some wiggle room could be put in s.t. near-zero stuff gets discarded
                ///current logic: if your ray starts at the edge of the sphere, or is tangent to the sphere, no intersect.
                if((x1 == x2) /*tangent*/ || (x1 <= 0 && x2 <= 0)){
                    return -1;
                }
                ///here, to decrease computation time, I can return a positive value if r is a ShadowRay. It doesn't need an exact location.
                else if((x2 < 0) || (0 < x1 && x1 < x2)) {
                    closest_x = x1;
                }
                else if((x1 < 0) || (0 < x2 && x2 < x1)) {        //remove test for speed
                    closest_x = x2;
                }
                return closest_x;
            }
}

//Fill in intersect struct if intersect is found, else return false
bool Sphere::intersects(PrimaryRay& r, Intersection* i) {
        float t = calculate_intersection(r);
        if(t < 0) return false;
        if((t > -EPSILON) && (t < 0)) t = 0;    //round up negative values of t within epsilon range to zero
        else {
            i->point = r.origin + r.direction * t;
            i->light = this->properties;
            i->normal = glm::normalize(i->point - this->center);
            i->distance = glm::length(i->point - r.origin);
            return true;
        }
}

///TODO put in some wiggle room s.t. being slightly under the surface of an object doesn't return an intersect
bool Sphere::intersects(ShadowRay& r) {
    if(calculate_intersection(r) < 0) return false;
    else return true;
}

