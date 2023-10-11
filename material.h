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

#endif // MATERIAL_H
