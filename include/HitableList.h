#ifndef HITABLELISTH
#define HITABLELISTH

#include "Hitable.h"
#include <vector>

using namespace std;

class HitableList : public Hitable
{
public:
    HitableList() {}
    HitableList(vector<Hitable*>& l) : list(l) {}
    // ~HitableList()
    // {
    //     for (int i = 0; i < list.size(); i++)
    //     {
    //         delete list[i];
    //         list[i] = nullptr;
    //     }
    // }

    bool hit(const ray& r, float tMin, float tMax, HitRecord& rec) const;

    vector<Hitable*> list;
};

bool HitableList::hit(const ray& r, float tMin, float tMax, HitRecord& rec) const
{
    HitRecord tempRec;

    bool hitAnything = false;
    double closestSoFar = tMax;

    for (Hitable* pHitable : list)
    {
        if (pHitable->hit(r, tMin, closestSoFar, tempRec))
        {
            hitAnything = true;
            closestSoFar = tempRec.t;
            rec = tempRec;
        }
    }

    return hitAnything;
}

#endif
