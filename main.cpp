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

    auto ground_mat = make_shared<Lambertian>(color(0.5, 0.5, 0.5));
    world.add(make_shared<Sphere>(point3(0, -1000, 0), 1000, ground_mat));

    for (int a = -11; a < 11; a++) {
        for (int b = -11; b < 11; b++) {
            auto choose_mat = random_double();
            point3 center(a + 0.9*random_double(), 0.2, b +0.9*random_double());

            if ((center - point3(4, 0.2, 0)).length() > 0.9) {
                shared_ptr<Material> sphere_mat;

                if (choose_mat < 0.8) {
                    // Diffuse
                    color albedo = random_vec3() * random_vec3();
                    sphere_mat = make_shared<Lambertian>(albedo);
                } else if (choose_mat < 0.95) {
                    // Metal
                    color albedo = random_vec3(0.5, 1);
                    double fuzz = random_double(0, 0.5);
                    sphere_mat = make_shared<Metal>(albedo, fuzz);
                } else {
                    // Glass
                    sphere_mat = make_shared<Dielectric>(1.5);
                }
                world.add(make_shared<Sphere>(center, 0.2, sphere_mat));
            }
        }
    }

    auto yellow_diffuse = make_shared<Lambertian>(color(0.8, 0.8, 0.0));
    auto pink_diffuse = make_shared<Lambertian>(color(0.7, 0.3, 0.3));
    auto clear_metal = make_shared<Metal>(color(0.8, 0.8, 0.8), 0.0);
    auto golden_metal = make_shared<Metal>(color(0.8, 0.6, 0.2), 0.3);
    auto dielectric = make_shared<Dielectric>(1.5);

    // Ground
    world.add(make_shared<Sphere>(point3(0.0, -100.5, -1.0), 100.0, yellow_diffuse));
    // Center
    world.add(make_shared<Sphere>(point3(0.0, 0.0, -1.0), 0.5, dielectric));  // Set negative radius for hollow glass sphere
    // Left
    world.add(make_shared<Sphere>(point3(-1.0, 0.0, -1.0), 0.5, clear_metal));
    // Right
    world.add(make_shared<Sphere>(point3(1.0, 0.0, -1.0), 0.5, golden_metal));

    // Camera
    Camera cam;
    cam.set_aspect_ratio(16.0 / 9.0);
    cam.set_image_width(1200);
    cam.set_samples_per_pixel(10); // 500
    cam.set_max_depth(50);

    cam.vfov_deg = 20;
    cam.lookfrom = point3(13, 2, 3);
    cam.lookat = point3(0, 0, 0);
    cam.vup = vec3(0, 1, 0);

    cam.defocus_angle_deg = 0.6;
    cam.focus_dist = 10.0;

    // Output .ppm image
    cam.render(std::cout, world);
}
