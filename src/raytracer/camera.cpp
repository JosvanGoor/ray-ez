#include "camera.hpp"

namespace raytracer
{

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