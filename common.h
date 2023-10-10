#ifndef COMMON_H
#define COMMON_H

#include <cmath>
#include <limits>
#include <memory>
#include <cstdlib>


using std::shared_ptr;
using std::make_shared;

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions
inline double deg2rad(double deg) {
    return deg * pi / 180.0;
}

inline double random_double() {
    // Return a random real in [0, 1)
    return rand() / (RAND_MAX + 1.0);
}

inline double random_double(double min, double max) {
    return min + (max - min) * random_double();
}

// Common Headers
#include "interval.h"
#include "ray.h"
#include "vec3.h"
#include "color.h"

vec3 random_vec3() {
    return vec3(random_double(), random_double(), random_double());
}

vec3 random_vec3(double min, double max) {
    return vec3(random_double(min, max), random_double(min, max), random_double(min, max));
}

vec3 random_nonzero_vec3_inside_unit_sphere() {
    while (true) {
        vec3 r = random_vec3(-1, 1);
        auto len_sqr = r.length_squared();
        if (len_sqr < 1 && len_sqr > 0.0001)
            return r;
    }
}

vec3 random_unit_vector() {
    return unit_vector(random_nonzero_vec3_inside_unit_sphere());
}

vec3 random_vec3_on_hemisphere(const vec3& normal) {
    vec3 v = random_unit_vector();
    return dot(v, normal) > 0.0 ? v : -v;
}

#endif // COMMON_H
