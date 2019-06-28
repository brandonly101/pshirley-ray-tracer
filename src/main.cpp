#include <string>
#include <iostream>
#include "vec3.h"

using namespace std;

int main(int argc, char const *argv[])
{
    vec3 test(1.0, 3.0, 7.0);
    cout << "Lmao ";
    cout << to_string(test.x()) + " ";
    cout << to_string(test.y()) + " ";
    cout << to_string(test.z()) << endl;
    return 0;
}
