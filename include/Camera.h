#ifndef CAMERAH
#define CAMERAH

#include "vec3.h"
#include "ray.h"

class Camera
{
public:
    Camera()
    {
        origin = vec3(0.0, 0.0, 0.0);
        lowLeftCorner = vec3(-1.77777777778, -1.0, -1.0);
        horizontal = vec3(1.77777777778 * 2.0, 0.0, 0.0);
        vertical = vec3(0.0, 2.0, 0.0);
    }

    ray getRay(float u, float v)
    {
        return ray(origin, lowLeftCorner + u * horizontal + v * vertical);
    }

    vec3 origin;
    vec3 lowLeftCorner;
    vec3 horizontal;
    vec3 vertical;
};

#endif
