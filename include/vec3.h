#ifndef VEC3H
#define VEC3H

#include <cmath>
#include <cstdlib>
#include <iostream>

using namespace std;

class vec3
{
public:
    static inline vec3 unit_vector(vec3 v);

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

inline vec3 vec3::unit_vector(vec3 v)
{
    return v / v.length();
}

#endif
