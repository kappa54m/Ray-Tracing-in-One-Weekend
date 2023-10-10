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

#endif // COMMON_H
