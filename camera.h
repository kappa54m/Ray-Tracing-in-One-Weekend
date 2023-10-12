#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"
#include "hittable.h"
#include "material.h"

#include <iostream>

class Camera {
public:
    Camera() {}

    void render(std::ostream& ostream, const Hittable& world) {
        _prepare();
        _render(ostream, world);
    }
private:
    void _prepare() {
        m_center = lookfrom;

        double focal_length = (lookfrom - lookat).length();
        double vp_height = 2.0;
        double vp_width = vp_height * (static_cast<double>(m_image_width) / m_image_height);

        m_cam_z = unit_vector(lookfrom - lookat);
        m_cam_x = unit_vector(cross(vup, m_cam_z));
        m_cam_y = cross(m_cam_z, m_cam_x);

        vec3 vp_u = vp_width * m_cam_x;
        vec3 vp_v = vp_height * -m_cam_y;

        // pixel to pixel dist within vp
        m_vp_du = vp_u / static_cast<double>(m_image_width);
        m_vp_dv = vp_v / static_cast<double>(m_image_height);

        vec3 vp_ul = m_center - (focal_length * m_cam_z) - (0.5 * vp_u) - (0.5 * vp_v);
        m_pixel00_loc = vp_ul + (0.5 * m_vp_du + 0.5 * m_vp_dv);
    }

    void _render(std::ostream& ostream, const Hittable& world) {
        ostream <<"P3\n" << m_image_width << ' ' << m_image_height << "\n255\n";

        for (int j = 0; j < m_image_height; j++) {
            std::clog << "\rScanlines remaining: " << (m_image_height - j) << ' ' << std::flush;
            for (int i = 0; i < m_image_width; i++) {
                color pixel_col(0.0, 0.0, 0.0);
                for (int i_samp = 0; i_samp < m_samples_per_pixel; i_samp++) {
                    Ray r = _sample_ray(i, j);
                    pixel_col += _ray_color(r, world);
                }

                write_color(ostream, pixel_col, m_samples_per_pixel);
            }
        }
        std::clog << "\rDone.             \n";
    }

    Ray _sample_ray(int i, int j) {
        point3 pixel_center = m_pixel00_loc + (i * m_vp_du) + (j * m_vp_dv);
        point3 pixel_samp = pixel_center + ((-0.5 + random_double()) * m_vp_du) + ((-0.5 + random_double()) * m_vp_dv);
        return Ray(m_center, pixel_samp - m_center);
    }

    color _ray_color(const Ray& r, const Hittable& world, int depth) const {
        if (depth > m_max_depth)
            return color(0, 0, 0);

        HitRecord hit;
        if (world.hit(r, Interval(0.001, infinity), hit)) {
            // Bounce back 50%
            Ray scattered;
            color attenuation;
            if (hit.material->scatter(r, hit, attenuation, scattered)) {
                return attenuation * _ray_color(scattered, world, depth+1);
            } else {
                return color(0, 0, 0);
            }
        } else { // Nothing hit
            vec3 unit_dir = unit_vector(r.direction());
            double a = 0.5 * (unit_dir.y() + 1.0);
            color white(1.0, 1.0, 1.0);
            color blue(0.5, 0.7, 1.0);
            return a * blue + (1 - a) * white;
        }
    }

    color _ray_color(const Ray& r, const Hittable& world) const {
        return _ray_color(r, world, 0);
    }

    void _update_image_height() {
        m_image_height = static_cast<int>(m_image_width / m_aspect_ratio);
        m_image_height = (m_image_height < 1) ? 1 : m_image_height;
    }

private:
    double m_aspect_ratio = 1.0;
    int m_image_width = 100;
    int m_image_height = -1;
    point3 m_center;
    point3 m_pixel00_loc;
    // Pixel delta u, v
    vec3 m_vp_du;
    vec3 m_vp_dv;
    int m_max_depth = 10;

    int m_samples_per_pixel = 4;

    vec3 m_cam_x, m_cam_y, m_cam_z; // Camera frame basis vectors
public:
    double vfov = 90;					// Vertical view angle (field of view; NOT USED FOR NOW)
    point3 lookfrom = point3(0, 0, -1); // Camera position
    point3 lookat = point3(0, 0, 0);    // Point camera is looking at
    vec3 vup = vec3(0, 1, 0);			// Camera-relative up direction
public:
    bool set_image_width(int w) {
        if (w < 0)
            return false;
        m_image_width = w;
        _update_image_height();
        return true;
    }
    bool set_aspect_ratio(double v) {
        if (v < 0) return false;
        m_aspect_ratio = v;
        _update_image_height();
        return true;
    }
    bool set_samples_per_pixel(int v) {
        if (v < 1) return false;
        m_samples_per_pixel = v;
        return true;
    }
    bool set_max_depth(int d) {
        if (d < 1) return false;
        m_max_depth = d;
        return true;
    }
    int get_image_width() const { return m_image_width; }
    int get_image_height() const { return m_image_height; }
    double get_aspect_ratio() const { return m_aspect_ratio; }
    int get_max_depth() const { return m_max_depth; }
};

#endif // CAMERA_H
