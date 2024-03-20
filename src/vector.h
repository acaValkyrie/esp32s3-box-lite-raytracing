#ifndef VECTOR_H
#define VECTOR_H

class Vector3
{
public:
    float x;
    float y;
    float z;
    Vector3(float x, float y, float z)
    {
        this->x = x;
        this->y = y;
        this->z = z;
    }
    Vector3()
    {
        this->x = 0;
        this->y = 0;
        this->z = 0;
    }
};

#endif