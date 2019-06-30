#include <string>
#include <iostream>
#include "vec3.h"

using namespace std;

const int nx = 960;
const int ny = 540;

int main(int argc, char const *argv[])
{
    cout << "P3" << endl;
    cout << to_string(nx) + " " + to_string(ny) << endl;
    cout << "255" << endl;

    for (int j = ny - 1; j >= 0; j--)
    {
        for (int i = 0; i < nx; i++)
        {
            vec3 col(
                float(i) / float(nx),
                float(j) / float(ny),
                0.2
            );

            int ir = int(256.0 * col[0]);
            int ig = int(256.0 * col[1]);
            int ib = int(256.0 * col[2]);

            cout << ir << " " << ig << " " << ib << endl;
        }
    }
}
