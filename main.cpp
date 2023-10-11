#include "common.h"
#include "camera.h"
#include "hittable.h"
#include "hittable_list.h"
#include "sphere.h"
#include "material.h"

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
    // World
    HittableList world;

    auto yellow_diffuse = make_shared<Lambertian>(color(0.8, 0.8, 0.0));
    auto pink_diffuse = make_shared<Lambertian>(color(0.7, 0.3, 0.3));
    auto clear_metal = make_shared<Metal>(color(0.8, 0.8, 0.8), 0.0);
    auto golden_metal = make_shared<Metal>(color(0.8, 0.6, 0.2), 0.3);
    auto dielectric = make_shared<Dielectric>(1.5);

    // Ground
    world.add(make_shared<Sphere>(point3(0.0, -100.5, -1.0), 100.0, yellow_diffuse));
    // Center
    world.add(make_shared<Sphere>(point3(0.0, 0.0, -1.0), 0.5, dielectric));
    // Left
    world.add(make_shared<Sphere>(point3(-1.0, 0.0, -1.0), 0.5, clear_metal));
    // Right
    world.add(make_shared<Sphere>(point3(1.0, 0.0, -1.0), 0.5, golden_metal));

    // Camera
    Camera cam;
    cam.set_aspect_ratio(16.0 / 9.0);
    cam.set_image_width(400);
    cam.set_samples_per_pixel(100);
    //cam.set_max_depth(50);

    // Output .ppm image
    cam.render(std::cout, world);
}
