#ifndef RECTANGLE_H
#define RECTANGLE_H

#include "DrawableObject.h"

namespace puddi
{
    class Rectangle : public DrawableObject
    {
    public:
        static void Init();
        Rectangle(Object *par);

    private:
        static void createPrototypeVertices();
    };
}

#endif
