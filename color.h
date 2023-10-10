#ifndef COLOR_H
#define COLOR_H

#include "vec3.h"
#include "interval.h"

#include <iostream>


using color = vec3;

void write_color(std::ostream& out, color pixel_color, int samples_per_pixel) {
    auto r = pixel_color[0];
    auto g = pixel_color[1];
    auto b = pixel_color[2];

    auto scale = 1.0 / samples_per_pixel;
    r *= scale;
    g *= scale;
    b *= scale;

    static const Interval INTENSITY(0.000, 0.999);
    // Write the translated [0,255] value of each color component.
    out << static_cast<int>(256 * INTENSITY.clamp(r)) << ' '
        << static_cast<int>(256 * INTENSITY.clamp(g)) << ' '
        << static_cast<int>(256 * INTENSITY.clamp(b)) << '\n';
}

void write_color(std::ostream& out, color pixel_color) {
    return write_color(out, pixel_color, 1);
}


#endif // COLOR_H
