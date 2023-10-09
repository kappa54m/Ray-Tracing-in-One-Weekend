#ifndef RAY_H
#define RAY_H

#include "vec3.h"


class Ray {
public:
    Ray() {}

    Ray(const point3& origin, const point3& dir) : orig(origin), dir(dir) {}

    point3 origin() const { return orig; }
    vec3 direction() const { return dir; }

    point3 at(double t) const {
        return orig + t * dir;
    }

private:
    point3 orig;
    vec3 dir;
};

#endif // RAY_H
