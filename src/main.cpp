#include <string>
#include <iostream>
#include <algorithm>
#include <cfloat>
#include <cstdlib>
#include <thread>

#include "vec3.h"
#include "ray.h"
#include "Sphere.h"
#include "HitableList.h"
#include "Camera.h"

using namespace std;

#define nx 960
#define ny 540
#define ns 100

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

// For multi-threading needs ... create a function with
// things passed in to work on.
void processPixels(int start,
                   int end,
                   Camera* cam,
                   Hitable* world,
                   vector<string>* pixels)
{
    for (int iter = start; iter < end; iter++)
    {
        const int i = iter % nx;
        const int j = ny - iter / nx - 1;

        vec3 col;

        for (int s = 0; s < ns; s++)
        {
            float u = float(i + drand48()) / float(nx);
            float v = float(j + drand48()) / float(ny);
            ray r = cam->getRay(u, v);
            vec3 p = r.pointAtParameter(2.0);
            col += getColor(r, world);
        }

        col /= float(ns);

        int ir = int(255.99 * sqrt(col[0]));
        int ig = int(255.99 * sqrt(col[1]));
        int ib = int(255.99 * sqrt(col[2]));

        (*pixels)[iter] = to_string(ir) + " " + to_string(ig) + " " + to_string(ib);
    }
}

int main(int argc, char const *argv[])
{
    cout << "P3" << endl;
    cout << to_string(nx) + " " + to_string(ny) << endl;
    cout << "255" << endl;

    vector<Hitable*> list;
    list.push_back(new Sphere(vec3(0, 0, -1), 0.5));
    list.push_back(new Sphere(vec3(0, -500000.5, -1), 500000));

    HitableList world(list);
    Camera cam;
    vector<string> pixels(nx * ny, "0 0 0");

    // processPixels(0, nx * ny, &cam, (Hitable*)&world, &pixels);

    const unsigned NUM_THREADS = thread::hardware_concurrency();

    vector<thread> threads(NUM_THREADS);
    for (int i = 0; i < NUM_THREADS; i++)
    {
        threads[i] = thread(
            processPixels,
            (i * nx * ny) / (int)NUM_THREADS,
            ((i + 1) * (nx * ny)) / (int)NUM_THREADS,
            &cam,
            (Hitable*)&world,
            &pixels
        );
    }
    for (int i = 0; i < NUM_THREADS; i++)
    {
        threads[i].join();
    }

    for (string& s : pixels) cout << s << endl;

    return 0;
}
