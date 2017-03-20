#include "pointlight.hpp"

namespace raytracer
{

    Vector3d PointLight::color() const { return m_color; }
    Vector3d PointLight::position() const { return m_position; }

    std::string PointLight::to_string() const
    {
        std::string s = "raytracer::PointLight\n";
        s += "    Color: " + m_color.to_string() + "\n";
        s += "    Position: " + m_position.to_string() + "\n";
        return s;
    }

}