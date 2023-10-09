#ifndef HITTABLE_LIST_H
#define HITTABLE_LIST_H

#include "hittable.h"

#include <memory>
#include <vector>

using std::shared_ptr;
using std::make_shared;

class HittableList : public Hittable {
public:

    HittableList() {}
    HittableList(shared_ptr<Hittable> object) { add(object); }

    void add(shared_ptr<Hittable> object) {
        objects.push_back(object);
    }
    void clear() { objects.clear(); }

    bool hit(const Ray& r, double ray_tmin, double ray_tmax, HitRecord& rec) const override {
        HitRecord temp_rec;
        bool hit_anything = false;
        double closest_so_far = ray_tmax;

        for (const auto& object : objects) {
            if (object->hit(r, ray_tmin, closest_so_far, temp_rec)) {
                hit_anything = true;
                closest_so_far = temp_rec.t;
                rec = temp_rec;
            }
        }
        return hit_anything;
    }

private:
    std::vector<shared_ptr<Hittable>> objects;
};

#endif // HITTABLE_LIST_H
