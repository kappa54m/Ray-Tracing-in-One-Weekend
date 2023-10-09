#include "common.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

#include <iostream>


color ray_color(const Ray& r, const Hittable& world) {
    HitRecord rec;
    if (world.hit(r, 0.0, infinity, rec)) {
        return 0.5 * (color(1.0, 1.0, 1.0) + rec.normal);
    } else {
        vec3 unit_dir = unit_vector(r.direction());
        double a = 0.5 * (unit_dir.y() + 1.0);
        color white(1.0, 1.0, 1.0);
        color blue(0.5, 0.7, 1.0);
        return a * blue + (1 - a) * white;
    }
}


int main()
{
    // Image
    double aspect_ratio = 16.0 / 9.0; // ideal ratio

    int image_width = 400;
    int image_height = static_cast<int>(image_width / aspect_ratio);
    image_height = (image_height < 1) ? 1 : image_height;

    // World
    HittableList world;

    world.add(make_shared<Sphere>(point3(0, 0, -1), 0.5));
    world.add(make_shared<Sphere>(point3(0, -100.5, -1), 100));

    // Camera
    double focal_length = 1.0;
    double vp_height = 2.0;
    double vp_width = vp_height * (static_cast<double>(image_width) / image_height);
    point3 cam_center(0, 0, 0);

    vec3 vp_u(vp_width, 0, 0);
    vec3 vp_v(0, -vp_height, 0);

    // pixel to pixel dist within vp
    vec3 vp_du = vp_u / static_cast<double>(image_width);
    vec3 vp_dv = vp_v / static_cast<double>(image_height);

    vec3 vp_ul = cam_center - vec3(0, 0, focal_length) - (0.5 * vp_u) - (0.5 * vp_v);
    vec3 pixel00_loc = vp_ul + (0.5 * vp_du + 0.5 * vp_dv);

    // Output .ppm image
    std::cout <<"P3\n" << image_width << ' ' << image_height << "\n255\n";

    for (int j = 0; j < image_height; j++) {
        std::clog << "\rScanlines remaining: " << (image_height - j) << ' ' << std::flush;
        for (int i = 0; i < image_width; i++) {
            point3 pixel_center = pixel00_loc + (i * vp_du) + (j * vp_dv);
            vec3 Ray_dir = pixel_center - cam_center;
            Ray r(cam_center, Ray_dir);

            color pixel_col = ray_color(r, world);
            write_color(std::cout, pixel_col);
        }
    }
    std::clog << "\rDone.             \n";
}
