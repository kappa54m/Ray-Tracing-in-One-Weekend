#ifndef COMMON_H
#define COMMON_H

#include <cmath>
#include <limits>
#include <memory>


using std::shared_ptr;
using std::make_shared;

// Constants
const double infinity = std::numeric_limits<double>::infinity();
const double pi = 3.1415926535897932385;

// Utility Functions
inline double deg2rad(double deg) {
    return deg * pi / 180.0;
}

// Common Headers
#include "ray.h"
#include "vec3.h"
#include "color.h"

#endif // COMMON_H
