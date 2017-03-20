#include "camera.hpp"

namespace raytracer
{

    void Camera::set_image(size_t w, size_t h, size_t ss)
    {
        m_image_width = w;
        m_image_height = h;
        m_supersamples = ss;
    }

    size_t Camera::image_width() const { return m_image_width; }
    size_t Camera::image_height() const { return m_image_height; }
    size_t Camera::supersamples() const {return m_supersamples; }
    Vector3d Camera::up() const { return m_up; }
    Vector3d Camera::eye() const { return m_eye; }
    Vector3d Camera::center() const { return m_center; }
    bool Camera::depth_of_field() const { return m_depth_of_field; }
    double Camera::aperture_radius() const { return m_aperture_radius; }
    size_t Camera::aperture_samples() const { return m_aperture_samples; }

    std::string Camera::to_string() const
    {
        std::string s = "raytracer::Camera\n";
        s += "    up: " + m_up.to_string() + "\n";
        s += "    eye: " + m_eye.to_string() + "\n";
        s += "    center: " + m_center.to_string() + "\n";
        if(m_depth_of_field)
        {
            s += "    depth of field enabled:\n";
            s += "        aperture radius: " + std::to_string(m_aperture_radius) + "\n";
            s += "        aperture samples: " + std::to_string(m_aperture_samples);
        }
        else s += "    depth of field disabled";
        return s;
    }

}