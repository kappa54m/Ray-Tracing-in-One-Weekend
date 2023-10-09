#ifndef CAMERA_H
#define CAMERA_H

#include "common.h"
#include "hittable.h"

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
        double focal_length = 1.0;
        double vp_height = 2.0;
        double vp_width = vp_height * (static_cast<double>(m_image_width) / m_image_height);

        vec3 vp_u(vp_width, 0, 0);
        vec3 vp_v(0, -vp_height, 0);

        // pixel to pixel dist within vp
        m_vp_du = vp_u / static_cast<double>(m_image_width);
        m_vp_dv = vp_v / static_cast<double>(m_image_height);

        vec3 vp_ul = m_center - vec3(0, 0, focal_length) - (0.5 * vp_u) - (0.5 * vp_v);
        m_pixel00_loc = vp_ul + (0.5 * m_vp_du + 0.5 * m_vp_dv);
    }

    void _render(std::ostream& ostream, const Hittable& world) {
        ostream <<"P3\n" << m_image_width << ' ' << m_image_height << "\n255\n";

        for (int j = 0; j < m_image_height; j++) {
            std::clog << "\rScanlines remaining: " << (m_image_height - j) << ' ' << std::flush;
            for (int i = 0; i < m_image_width; i++) {
                point3 pixel_center = m_pixel00_loc + (i * m_vp_du) + (j * m_vp_dv);
                vec3 Ray_dir = pixel_center - m_center;
                Ray r(m_center, Ray_dir);

                color pixel_col = _ray_color(r, world);
                write_color(ostream, pixel_col);
            }
        }
        std::clog << "\rDone.             \n";
    }

    color _ray_color(const Ray& r, const Hittable& world) const {
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

public:
    bool set_image_width(int w) {
        if (w < 0)
            return false;
        m_image_width = w;
        _update_image_height();
        return true;
    }
    bool set_aspect_ratio(double v) {
        if (v < 0)
            return false;
        m_aspect_ratio = v;
        _update_image_height();
        return true;
    }

    int get_image_width() const { return m_image_width; }
    int get_image_height() const { return m_image_height; }

    double get_aspect_ratio() const { return m_aspect_ratio; }
};

#endif // CAMERA_H
