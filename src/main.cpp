#include <string>
#include <iostream>
#include <algorithm>
#include <cfloat>
#include <cstdlib>
#include <thread>
#include <ctime>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb_image_write.h"

#include "vec3.h"
#include "ray.h"
#include "Sphere.h"
#include "HitableList.h"
#include "Camera.h"

using namespace std;

// Constants that define properties of this ray tracer.
#define N_X 1280
#define N_Y 720
#define N_CHANNELS 4
#define N_S 100

double getRand()
{
	return double(rand()) / double(RAND_MAX);
}

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
            vec3 p = r.pointAtParameter(2.0);
            col += getColor(r, world);
        }

        col /= float(N_S);

        int ir = int(255.99 * sqrt(col[0]));
        int ig = int(255.99 * sqrt(col[1]));
        int ib = int(255.99 * sqrt(col[2]));

		(*pixels)[iter * N_CHANNELS + 0] = (unsigned char)ir;
		(*pixels)[iter * N_CHANNELS + 1] = (unsigned char)ig;
		(*pixels)[iter * N_CHANNELS + 2] = (unsigned char)ib;
		if (N_CHANNELS == 4)
		{
			(*pixels)[iter * N_CHANNELS + 3] = 0xFF;
		}
    }
}

int main(int argc, char const *argv[])
{
	clock_t begin = clock();

	cout << "Ray tracing image ..." << endl;

    vector<Hitable*> list;
    list.push_back(new Sphere(vec3(0, 0, -1), 0.5));
    list.push_back(new Sphere(vec3(0, -500000.5, -1), 500000));

    HitableList world(list);
    Camera cam;
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

	clock_t end = clock();
	double elapsedSecs = double(end - begin) / CLOCKS_PER_SEC;
	cout << "Time elapsed: " + to_string(int(elapsedSecs) / 60) + "m" + to_string(int(elapsedSecs) % 60) + "s" << endl;

    return 0;
}
