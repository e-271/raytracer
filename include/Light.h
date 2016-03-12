#ifndef LIGHT_H
#define LIGHT_H


class Light
{

    LightProperties lp;
    lp.ambient = vec3(0,0,0);
    lp.diffuse = vec3(0,0,0);
    lp.specular = vec3(0,0,0);
    lp.emission = vec3(0,0,0);
    lp.shininess = 1;
    public:
        Light();
        virtual ~Light();
    protected:
    private:
};

#endif // LIGHT_H
