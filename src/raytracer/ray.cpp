#include "ray.hpp"

namespace raytracer
{

    Vector3d Ray::at(double distance) const
    {
        return m_origin + (m_direction * distance);
    }

    std::string Ray::to_string() const
    {
        std::string s = "raytracer::Ray\n";
        s += "    origin: " + m_origin.to_string() + "\n";
        s += "    direction: " + m_direction.to_string() + "\n";
        return s;
    }

}