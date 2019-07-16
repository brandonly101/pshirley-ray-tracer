#include <string>
#include <iostream>
#include <algorithm>
#include <cfloat>
#include <cstdlib>
#include <thread>
#include <cmath>
#include <chrono>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "vec3.h"
#include "ray.h"
#include "Sphere.h"
#include "HitableList.h"
#include "Camera.h"
#include "Material.h"

using namespace std;

// Constants that define properties of this ray tracer.
#define N_X 1280
#define N_Y 720
#define N_CHANNELS 4
#define N_S 1024
#define N_BOUNCES 50

vec3 SchlickApprox(const vec3 n, const vec3 l, const vec3 F0)
{
    // More intuitively, lerp(F0, <white>, pow(1.0 - dot(n, l), 5.0))
    return F0 + (vec3(1, 1, 1) - F0) * pow(1.0 - max(0.0f, dot(n, l)), 5.0);
}

vec3 getColor(const ray& r, Hitable* world, int depth)
{
    HitRecord rec;
    if (depth < N_BOUNCES && world->hit(r, 0.001, FLT_MAX, rec))
    {
        vec3 color;
        vec3 attenuation;

        ray scattered;
        ray reflected;

        // Ray trace diffuse lambertian lighting
        if (rec.pMat->scatter(r, rec, attenuation, scattered))
        {
            color += attenuation * getColor(scattered, world, depth + 1);
        }

        // Ray trace reflected specular lighting
        if (depth < N_BOUNCES - 1 && rec.pMat->reflect(r, rec, attenuation, reflected))
        {
            vec3 F = SchlickApprox(vec3::normalize(rec.normal), vec3::normalize(reflected.direction()), attenuation);

            // float NoL = dot(vec3::normalize(rec.normal), vec3::normalize(reflected.direction()));
            // F /= 4.0 * NoL * NoL;
            vec3 refLightColor = getColor(reflected, world, depth + 1);

            color *= (vec3(1, 1, 1) - F); // Factor down diffuse component
            color += F * refLightColor; // Add specular contribution
        }

        return color;
    }
    else
    {
        // Sky background
        vec3 unitDirection = vec3::normalize(r.direction());
        float t = 0.5 * (unitDirection.y() + 1.0);
        // t = t * t;

        vec3 color = (1.0 - t) * vec3(1.0, 1.0, 1.0) + t * vec3(0.5, 0.7, 1.0);
        // if (depth != 0) color *= 2.0;
        // color *= 1.25;
        return color;
    }
}

// For multi-threading needs ... create a function with
// things passed in to work on.
void processPixels(int start,
                   int end,
                   Camera* cam,
                   Hitable* world,
                   vector<unsigned char>* pixels)
{
    for (int iter = start; iter < end; iter++)
    {
        const int i = iter % N_X;
        const int j = N_Y - iter / N_X - 1;

        vec3 col;

        for (int s = 0; s < N_S; s++)
        {
            float u = float(i + getRand()) / float(N_X);
            float v = float(j + getRand()) / float(N_Y);
            ray r = cam->getRay(u, v);
            col += getColor(r, world, 0);
        }

        col /= float(N_S);

        int ir = int(255.99 * (float)sqrt(clamp(col[0], 0.0f, 1.0f)));
        int ig = int(255.99 * (float)sqrt(clamp(col[1], 0.0f, 1.0f)));
        int ib = int(255.99 * (float)sqrt(clamp(col[2], 0.0f, 1.0f)));

        (*pixels)[iter * N_CHANNELS + 0] = (unsigned char)ir;
        (*pixels)[iter * N_CHANNELS + 1] = (unsigned char)ig;
        (*pixels)[iter * N_CHANNELS + 2] = (unsigned char)ib;
        if (N_CHANNELS == 4)
        {
            (*pixels)[iter * N_CHANNELS + 3] = 0xFF;
        }
    }
}

int main()
{
    auto start = chrono::steady_clock::now();

    cout << "Ray tracing image ..." << endl;

    vector<Hitable*> list;
    list.push_back(new Sphere(vec3(0, 0, -1), 0.5, new Lambertian(vec3(1.0, 0.25, 0.25))));
    list.push_back(new Sphere(vec3(0, -2500.5, -1), 2500, new Lambertian(vec3(0.8 , 0.8, 0))));
    // list.push_back(new Sphere(vec3(1, 0, -1), 0.5, new Lambertian(vec3(1.0, 0.7, 0.2))));
    // list.push_back(new Sphere(vec3(-1, 0, -1), 0.5, new Lambertian(vec3(0, 0, 0.8))));
    // list.push_back(new Sphere(vec3(1, 0, -1), 0.5, new Metal(vec3(0.8, 0.6, 0.2), 0.0)));
    // list.push_back(new Sphere(vec3(-1, 0, -1), 0.5, new Metal(vec3(0.8, 0.8, 0.8), 0.25)));
    list.push_back(new Sphere(vec3(1, 0, -1), 0.5, new CookTorrance(vec3(0.8, 0.6, 0.2), 0.0, 0)));
    list.push_back(new Sphere(vec3(-1, 0, -1), 0.5, new CookTorrance(vec3(0.8, 0.8, 0.8), 0.0, 0)));

    HitableList world(list);
    Camera cam(65, 16.0 / 9.0);
    vector<unsigned char> pixels(N_X * N_Y * N_CHANNELS, 0);

    // Create and kick off threads for subsections of pixels.
    const int NUM_THREADS = int(thread::hardware_concurrency());
    vector<thread> threads(NUM_THREADS);
    for (int i = 0; i < NUM_THREADS; i++)
    {
        threads[i] = thread(
            processPixels,
            (i * N_X * N_Y) / (int)NUM_THREADS,
            ((i + 1) * (N_X * N_Y)) / (int)NUM_THREADS,
            &cam,
            (Hitable*)&world,
            &pixels
        );
    }

    // Block until all threads are complete.
    for (int i = 0; i < NUM_THREADS; i++)
    {
        threads[i].join();
    }

    // Write to PNG file
    int x = N_X;
    int y = N_Y;
    int n = N_CHANNELS;
    stbi_write_png("image.png", x, y, n, pixels.data(), 0);

    cout << "Done!" << endl;

    auto end = chrono::steady_clock::now();

    // cout << "Elapsed time in nanoseconds : "
    //     << chrono::duration_cast<chrono::nanoseconds>(end - start).count()
    //     << " ns" << endl;

    // cout << "Elapsed time in microseconds : "
    //     << chrono::duration_cast<chrono::microseconds>(end - start).count()
    //     << " µs" << endl;

    // cout << "Elapsed time in milliseconds : "
    //     << chrono::duration_cast<chrono::milliseconds>(end - start).count()
    //     << " ms" << endl;

    double elapsedSecs = double(chrono::duration_cast<chrono::milliseconds>(end - start).count()) / 1000.0;

    // cout << "Time elapsed: " + to_string(int(elapsedSecs) / 60) + ":" + to_string(int(elapsedSecs) % 60) + "s" << endl;
    cout << "Time elapsed: " << to_string(elapsedSecs) << "s" << endl;

    return 0;
}
