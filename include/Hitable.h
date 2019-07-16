#ifndef HITABLEH
#define HITABLEH

#include "ray.h"

class Material;

struct HitRecord
{
    float t;
    vec3 p;
    vec3 normal;
    Material* pMat;
};

class Hitable
{
public:
    virtual ~Hitable() {}

    virtual bool hit(const ray& r, float tMin, float tMax, HitRecord& rec) const = 0;
};

#endif
