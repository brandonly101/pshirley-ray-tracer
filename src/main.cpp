#include <string>
#include <iostream>
#include "vec3.h"
#include "ray.h"

using namespace std;

const int nx = 960;
const int ny = 540;

vec3 getSkyColor(const ray& r)
{
    vec3 unitDirection = vec3::unit_vector(r.direction());
    float t = 0.5 * (unitDirection.y() + 1.0);

    // It's a LERP
    vec3 color = (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);

    return color;
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

    for (int j = ny - 1; j >= 0; j--)
    {
        for (int i = 0; i < nx; i++)
        {
            float u = float(i) / float(nx);
            float v = float(j) / float(ny);

            ray r(origin, lowLeftCorner + u * horizontal + v * vertical);

            vec3 col = getSkyColor(r);

            int ir = int(255.99 * col[0]);
            int ig = int(255.99 * col[1]);
            int ib = int(255.99 * col[2]);

            cout << ir << " " << ig << " " << ib << endl;
        }
    }

    return 0;
}
