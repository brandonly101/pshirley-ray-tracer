#ifndef VEC3H
#define VEC3H

#include <cmath>
#include <cstdlib>
#include <iostream>

using namespace std;

double getRand()
{
    return double(random()) / double(RAND_MAX);
}

class vec3
{
public:
    static inline vec3 normalize(vec3 v);
    static inline vec3 randomInUnitSphere();
    static inline vec3 reflect(const vec3& v, const vec3& n);
    static inline vec3 randomInUnitHemisphere(const vec3& n);

    vec3()
    {
        e[0] = 0;
        e[1] = 0;
        e[2] = 0;
    }
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

    inline const vec3& operator+() const { return *this; }
    inline vec3 operator-() const { return vec3(-e[0], -e[1], -e[2]); }
    inline float operator[](int i) const { return e[i]; }
    inline float& operator[](int i) { return e[i]; }

    inline vec3& operator+=(const vec3& v2);
    inline vec3& operator-=(const vec3& v2);
    inline vec3& operator*=(const vec3& v2);
    inline vec3& operator/=(const vec3& v2);

    inline vec3& operator*=(const float& t);
    inline vec3& operator/=(const float& t);

    inline float length() const
    {
        return sqrt(e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
    }

    inline float squared_length() const
    {
        return (e[0] * e[0] + e[1] * e[1] + e[2] * e[2]);
    }

    inline void normalize();

    float e[3];
};

inline istream& operator>>(istream& is, vec3& t)
{
    is >> t.e[0] >> t.e[1] >> t.e[2];
    return is;
}

inline ostream& operator<<(ostream& os, const vec3& t)
{
    os << t.e[0] << " " << t.e[1] << " " << t.e[2];
    return os;
}

inline void vec3::normalize()
{
    float k = 1.0 / this->length();
    (*this) *= k;
}

inline vec3 operator+(const vec3& a, const vec3& b)
{
    return vec3(a.e[0] + b.e[0], a.e[1] + b.e[1], a.e[2] + b.e[2]);
}

inline vec3 operator-(const vec3& a, const vec3& b)
{
    return vec3(a.e[0] - b.e[0], a.e[1] - b.e[1], a.e[2] - b.e[2]);
}

inline vec3 operator*(const vec3& a, const vec3& b)
{
    return vec3(a.e[0] * b.e[0], a.e[1] * b.e[1], a.e[2] * b.e[2]);
}

inline vec3 operator/(const vec3& a, const vec3& b)
{
    return vec3(a.e[0] / b.e[0], a.e[1] / b.e[1], a.e[2] / b.e[2]);
}

inline vec3 operator*(const float& a, const vec3& b)
{
    return vec3(a * b.e[0], a * b.e[1], a * b.e[2]);
}

inline vec3 operator/(const vec3& b, const float& a)
{
    return vec3(b.e[0] / a, b.e[1] / a, b.e[2] / a);
}

inline vec3 operator*(const vec3& a, const float& b)
{
    return vec3(a.e[0] * b, a.e[1] * b, a.e[2] * b);
}

inline float dot(const vec3& a, const vec3& b)
{
    return a[0] * b[0] + a[1] * b[1] + a[2] * b[2];
}

inline vec3 cross(const vec3& a, const vec3& b)
{
    return vec3(
        a.e[1] * b.e[2] - a.e[2] * b.e[1],
        a.e[0] * b.e[2] - a.e[2] * b.e[0],
        a.e[0] * b.e[1] - a.e[1] * b.e[0]
    );
}

inline vec3 vec3::reflect(const vec3& v, const vec3& n)
{
    return v - 2 * dot(v, n) * n;
}

inline vec3& vec3::operator+=(const vec3& a)
{
    e[0] += a.e[0];
    e[1] += a.e[1];
    e[2] += a.e[2];
    return *this;
}

inline vec3& vec3::operator-=(const vec3& a)
{
    e[0] -= a.e[0];
    e[1] -= a.e[1];
    e[2] -= a.e[2];
    return *this;
}

inline vec3& vec3::operator*=(const vec3& a)
{
    e[0] *= a.e[0];
    e[1] *= a.e[1];
    e[2] *= a.e[2];
    return *this;
}

inline vec3& vec3::operator/=(const vec3& a)
{
    e[0] /= a.e[0];
    e[1] /= a.e[1];
    e[2] /= a.e[2];
    return *this;
}

inline vec3& vec3::operator*=(const float& a)
{
    e[0] *= a;
    e[1] *= a;
    e[2] *= a;
    return *this;
}

inline vec3& vec3::operator/=(const float& a)
{
    e[0] /= a;
    e[1] /= a;
    e[2] /= a;
    return *this;
}

inline vec3 vec3::normalize(vec3 v)
{
    return v / v.length();
}

inline vec3 vec3::randomInUnitSphere()
{
    vec3 p;

    // do
    // {
    //     p = 2.0 * vec3(getRand(), getRand(), getRand()) - vec3(1, 1, 1);
    // }
    // while (p.squared_length() >= 1.0);

    p = (2.0 * vec3(getRand(), getRand(), getRand())) - vec3(1, 1, 1);
    p.normalize();
    p *= pow(getRand(), 1.0 / 3.0);

    // float theta = M_PI * getRand();
    // float phi = 2.0 * M_PI * getRand();
    // float r = getRand();

    // float x = r * sin(theta) * cos(phi);
    // float y = r * sin(theta) * sin(phi);
    // float z = r * cos(theta);

    // p = vec3(x, y, z);

    return p;
}

inline vec3 vec3::randomInUnitHemisphere(const vec3& n)
{
    vec3 p;

    float theta = M_PI / 2.0 * getRand();
    float phi = 2.0 * M_PI * getRand();

    float x = sin(theta) * cos(phi);
    float y = sin(theta) * sin(phi);
    float z = cos(theta);

    p = vec3(x, y, z);

    vec3 Up = vec3(0, 0, 1);
    vec3 t = cross(Up, n);
    vec3 b = cross(n, t);

    x = dot(vec3(t[0], b[0], n[0]), p);
    y = dot(vec3(t[1], b[1], n[1]), p);
    z = dot(vec3(t[2], b[2], n[2]), p);

    p = vec3(x, y, z);

    return p;
}

inline vec3 lerp(const vec3& a, const vec3& b, float t)
{
    t = clamp(t, 0.0f, 1.0f);
    vec3 result;
    result = (1.0 - t) * a + t * b;
    return result;
}

#endif
