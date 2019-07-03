#include <string>
#include <iostream>
#include <algorithm>
#include <cfloat>

#include "vec3.h"
#include "ray.h"
#include "Sphere.h"
#include "HitableList.h"

using namespace std;

const int nx = 1280;
const int ny = 720;

vec3 getColor(const ray& r, Hitable* world)
{
    HitRecord rec;
    if (world->hit(r, 0.0, FLT_MAX, rec))
    {
        vec3 N = rec.normal;
        return 0.5 * vec3(N.x() + 1, N.y() + 1, N.z() + 1);
    }
    else
    {
        vec3 unitDirection = vec3::normalize(r.direction());
        float t = 0.5 * (unitDirection.y() + 1.0);

        // Sky background
        vec3 color = (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
        return color;
    }
}

int main(int argc, char const *argv[])
{
    cout << "P3" << endl;
    cout << to_string(nx) + " " + to_string(ny) << endl;
    cout << "255" << endl;

    vec3 lowLeftCorner(-1.77777777778, -1.0, -1.0);
    vec3 origin(0.0, 0.0, 0.0);
    vec3 horizontal(1.77777777778 * 2.0, 0.0, 0.0);
    vec3 vertical(0.0, 2.0, 0.0);

    vector<Hitable*> list;
    list.push_back(new Sphere(vec3(0, 0, -1), 0.5));
    list.push_back(new Sphere(vec3(0, -500000.5, -1), 500000));

    HitableList world(list);

    for (int j = ny - 1; j >= 0; j--)
    {
        for (int i = 0; i < nx; i++)
        {
            float u = float(i) / float(nx);
            float v = float(j) / float(ny);

            ray r(origin, lowLeftCorner + u * horizontal + v * vertical);

            vec3 col = getColor(r, (Hitable*)&world);

            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);

            cout << ir << " " << ig << " " << ib << endl;
        }
    }

    return 0;
}
