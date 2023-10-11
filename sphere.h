#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class Sphere : public Hittable {
public:
    Sphere(const point3& center, double radius, shared_ptr<Material> material)
        : center(center), radius(radius), material(material) {}

    bool hit(const Ray& r, Interval ray_t, HitRecord& hit) const override {
        vec3 oc = r.origin() - center;
        double a = dot(r.direction(), r.direction());
        double b = 2.0 * dot(oc, r.direction());
        double c = dot(oc, oc) - radius * radius;
        double d = b*b - 4*a*c;
        if (d < 0)
            return false;

        double sqrtd = sqrt(d);

        double t = (-b - sqrtd) / (2 * a);
        if (t < ray_t.min || t > ray_t.max) {
            t = (-b + sqrtd) / (2 * a);
            if (t < ray_t.min || t > ray_t.max)
                return false;
        }

        hit.t = t;
        hit.p = r.at(t);
        vec3 outward_normal = (hit.p - center) / radius;
        hit.set_face_normal(r, outward_normal);
        hit.material = material;

        return true;
    }
private:
    point3 center;
    double radius;
    shared_ptr<Material> material;
};

#endif // SPHERE_H
