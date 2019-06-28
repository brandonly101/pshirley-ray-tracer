#include <cmath>
#include <cstdlib>
#include <iostream>

class vec3
{
public:
    vec3() {}
    vec3(const float e0, const float e1, const float e2)
    {
        e[0] = e0;
        e[1] = e1;
        e[2] = e2;
    }
    inline float x() const { return e[0]; }
    inline float y() const { return e[1]; }
    inline float z() const { return e[2]; }
    inline float r() const { return e[0]; }
    inline float g() const { return e[1]; }
    inline float b() const { return e[2]; }

    float e[3];
};
