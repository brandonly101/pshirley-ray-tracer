#ifndef SPHEREH
#define SPHEREH

#include "Hitable.h"
#include "Material.h"

class Sphere : public Hitable
{
public:
    Sphere() : pMat(NULL) {}
    Sphere(vec3 center, float r, Material* pMatIn) : center(center), radius(r), pMat(pMatIn) {}
    virtual bool hit(const ray& r, float tMin, float tMax, HitRecord& rec) const;
    vec3 center;
    float radius;
    Material* pMat;
};

bool Sphere::hit(const ray& r, float tMin, float tMax, HitRecord& rec) const
{
    vec3 oc = r.origin() - center;

    float a = dot(r.direction(), r.direction());
    float b = 2.0 * dot(oc, r.direction());
    float c = dot(oc, oc) - radius * radius;

    float discriminant = b * b - 4 * a * c;
    if (discriminant > 0)
    {
        float t = (-b - sqrt(discriminant)) / (2.0 * a);

        if (tMin < t && t < tMax)
        {
            rec.t = t;
            rec.p = r.pointAtParameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.pMat = pMat;
            return true;
        }

        t = (-b + sqrt(discriminant)) / (2.0 * a);
        if (tMin < t && t < tMax)
        {
            rec.t = t;
            rec.p = r.pointAtParameter(rec.t);
            rec.normal = (rec.p - center) / radius;
            rec.pMat = pMat;
            return true;
        }
    }
    return false;
}

#endif
