#ifndef SPHERE_H
#define SPHERE_H

#include "hittable.h"

class Sphere : public Hittable {
public:
    Sphere(const point3& center, double radius) : center(center), radius(radius) {}

    bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const override {
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

        rec.t = t;
        rec.p = r.at(t);
        vec3 outward_normal = (rec.p - center) / radius;
        rec.set_face_normal(r, outward_normal);

        return true;
    }
private:
    point3 center;
    double radius;
};

#endif // SPHERE_H
