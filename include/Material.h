#ifndef MATERIALH
#define MATERIALH

#include "vec3.h"
#include <algorithm>

/**
 *
 * Abstract base class for materials.
 *
 */

class Material
{
public:
    virtual bool scatter(const ray& rayIn, const HitRecord& rec, vec3& attenuation, ray& scattered) const = 0;
    virtual bool reflect(const ray& rayIn, const HitRecord& rec, vec3& attenuation, ray& reflected) const = 0;
};



/**
 *
 * Material class for Lambertian materials, or materials with only a diffuse
 * component. Uses globally illuminated scattering to light objects at the
 * moment.
 *
 */

class Lambertian : public Material
{
public:
    Lambertian(const vec3& a = vec3(1, 1, 1)) : albedo(a) {}

    virtual bool scatter(const ray& rayIn, const HitRecord& rec, vec3& attenuation, ray& scattered) const
    {
        attenuation = albedo;

        vec3 target = rec.normal + vec3::randomInUnitSphere();
        scattered = ray(rec.p, target);

        // vec3 target = randomInUnitHemisphere(rec.normal);
        // scattered = ray(rec.p, target);
        // attenuation *= std::clamp(dot(rec.normal, target), 0.0f, 1.0f);

        // vec3 target = vec3::normalize(rec.normal + vec3::normalize(vec3::randomInUnitSphere()));
        // scattered = ray(rec.p, target);
        // attenuation *= std::clamp(dot(rec.normal, target), 0.0f, 1.0f);

        return true;
    }

    virtual bool reflect(const ray& rayIn, const HitRecord& rec, vec3& attenuation, ray& reflected) const { return false; }

    vec3 albedo;
};



/**
 *
 * Material class for metals. Unlike Lambertian materials, metal materials only
 * ever reflect light (they have no diffuse component). The albedo of the
 * material is thought of as the color of the metal. Takes in, as a parameter,
 * a roughness to determine how scattered the reflection is.
 *
 */

class Metal : public Material
{
public:
    Metal(const vec3& a = vec3(1, 1, 1), float r = 0) : albedo(a), roughness(std::clamp(r, 0.0f, 1.0f)) {}

    virtual bool scatter(const ray& rayIn, const HitRecord& rec, vec3& attenuation, ray& scattered) const { return false; }

    virtual bool reflect(const ray& rayIn, const HitRecord& rec, vec3& attenuation, ray& reflected) const
    {
        attenuation = albedo;
        vec3 vReflect = vec3::reflect(vec3::normalize(rayIn.direction()), rec.normal);
        reflected = ray(rec.p, vReflect + roughness * vec3::randomInUnitSphere());

        return dot(reflected.direction(), rec.normal) >= 0;
    }

    vec3 albedo;
    float roughness;
};



/**
 *
 * Material class for Cook-Torrance-based materials. Takes, as parameters,
 * an albedo, roughness, and metallicness to define said properties of
 * the material.
 *
 */

class CookTorrance : public Material
{
public:
    CookTorrance(const vec3& a = vec3(1, 1, 1), float r = 0, float m = 0) :
        albedo(a),
        roughness(std::clamp(r, 0.0f, 1.0f)),
        metallic(std::clamp(m, 0.0f, 1.0f))
    {}

    virtual bool scatter(const ray& rayIn, const HitRecord& rec, vec3& attenuation, ray& scattered) const
    {
        // Short-circuit scatter ray tracing if material is fully metallic because metals have no diffuse
        if (metallic == 1.0)
        {
            return false;
        }

        attenuation = albedo * (1.0 - metallic);

        vec3 target = rec.normal + vec3::randomInUnitSphere();
        scattered = ray(rec.p, target);

        // vec3 target = randomInUnitHemisphere(rec.normal);
        // scattered = ray(rec.p, target);
        // attenuation *= std::clamp(dot(rec.normal, target), 0.0f, 1.0f);

        // vec3 target = vec3::normalize(rec.normal + vec3::normalize(vec3::randomInUnitSphere()));
        // scattered = ray(rec.p, target);
        // attenuation *= std::clamp(dot(rec.normal, target), 0.0f, 1.0f);

        return true;
    }

    virtual bool reflect(const ray& rayIn, const HitRecord& rec, vec3& attenuation, ray& reflected) const
    {
        attenuation = lerp(vec3(0.04, 0.04, 0.04), albedo, metallic);
        vec3 vReflect = vec3::reflect(vec3::normalize(rayIn.direction()), rec.normal);
        reflected = ray(rec.p, vReflect + roughness * vec3::randomInUnitSphere());

        return dot(reflected.direction(), rec.normal) >= 0;
    }

    vec3 albedo;
    float roughness;
    float metallic;
};

#endif
