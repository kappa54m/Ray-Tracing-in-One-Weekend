#ifndef HITTABLE_H
#define HITTABLE_H

#include "common.h"
#include "interval.h"

class HitRecord {
public:
    point3 p;
    vec3 normal;
    shared_ptr<class Material> material;
    double t;
    bool front_face;

    // Set normal such that it points against incoming ray (r)
    void set_face_normal(const Ray& r, const vec3& outward_normal_unit) {
        front_face = dot(r.direction(), outward_normal_unit);
        normal = front_face ? outward_normal_unit : -outward_normal_unit;
    }
};

class Hittable {
public:
    virtual ~Hittable() = default;

    virtual bool hit(const Ray& r, Interval ray_t, HitRecord& rec) const = 0;
};

#endif // HITTABLE_H
