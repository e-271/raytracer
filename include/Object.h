#ifndef OBJECT_H
#define OBJECT_H

#import "variables.h"

class Object
{
    public:
        mat4 transforms;

        Object::Object(int, int, lightProperties, float*)
        virtual ~Object();
    protected:
    private:
};

#endif // OBJECT_H
