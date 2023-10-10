#include "common.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"

#include <iostream>


color ray_color(const Ray& r, const Hittable& world) {
    HitRecord rec;
    if (world.hit(r, Interval(0.0, infinity), rec)) {
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
    Camera cam;
    cam.set_aspect_ratio(16.0 / 9.0);
    cam.set_image_width(400);
    cam.set_samples_per_pixel(100);

    // Output .ppm image
    cam.render(std::cout, world);
}
