#ifndef MATERIAL_H
#define MATERIAL_H

#include "common.h"
#include "hittable.h"


class Material {
public:
    virtual ~Material() = default;

    virtual bool scatter(const Ray& r_in, const HitRecord& hit, color& attenuation, Ray& scattered) const = 0;
};

class Lambertian : public Material {
public:
    Lambertian(const color& albedo) : m_albedo(albedo) {}

    bool scatter(const Ray& r_in, const HitRecord& hit, color& attenuation, Ray& scattered) const override {
        vec3 bounce_dir = hit.normal + random_unit_vector();
        if (bounce_dir.near_zero())
            bounce_dir = hit.normal;
        scattered = Ray(hit.p, bounce_dir);
        attenuation = m_albedo;
        return true;
    }
private:
    color m_albedo;
};

class SoftDiffuse : public Material {
public:
    SoftDiffuse(const color& albedo) : m_albedo(albedo) {}

    bool scatter(const Ray& r_in, const HitRecord& hit, color& attenuation, Ray& scattered) const override final {
        vec3 bounce_dir = random_vec3_on_hemisphere(hit.normal);
        scattered = Ray(hit.p, bounce_dir);
        attenuation = m_albedo;
        return true;
    }
private:
    color m_albedo;
};

class Metal : public Material {
public:
    Metal(const color& albedo, double fuzz) : m_albedo(albedo), m_fuzz(fuzz) {}

    bool scatter(const Ray& r_in, const HitRecord& hit, color& attenuation, Ray& scattered) const override {
        vec3 reflected = reflect(unit_vector(r_in.direction()), hit.normal);
        vec3 scattered_dir = reflected + m_fuzz*random_unit_vector();
        scattered = Ray(hit.p, scattered_dir);
        attenuation = m_albedo;
        return dot(scattered_dir, hit.normal) > 0;
    }
private:
    color m_albedo;
    double m_fuzz;
};

class Dielectric : public Material {
public:
    Dielectric(double index_of_refraction) : m_ir(index_of_refraction) {}

    bool scatter(const Ray& r_in, const HitRecord& hit, color& attenuation, Ray& scattered) const override {
        attenuation = color(1.0, 1.0, 1.0);  // Perfect glass
        auto refraction_ratio = hit.front_face ? 1.0/m_ir : m_ir/1.0;

        vec3 ray_dir_unit = unit_vector(r_in.direction());
        double cos_theta = fmin(dot(-ray_dir_unit, hit.normal), 1.0);
        double sin_theta = sqrt(1 - cos_theta * cos_theta);

        vec3 direction;
        if (refraction_ratio * sin_theta > 1.0  // Over critical angle
                || _schlick_approximation(cos_theta, refraction_ratio) > random_double())
            direction = reflect(ray_dir_unit, hit.normal);
        else
            direction = refract(ray_dir_unit, hit.normal, refraction_ratio);
        scattered = Ray(hit.p, direction);
        return true;
    }

private:
    static double _schlick_approximation(double cosine, double ref_idx) {
        auto r0 = (1 - ref_idx) / (1 + ref_idx);
        r0 = r0 * r0;
        return r0 + (1 - r0) * pow((1 - cosine), 5);
    }
private:
    double m_ir;
};

#endif // MATERIAL_H
