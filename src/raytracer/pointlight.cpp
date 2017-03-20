#include "pointlight.hpp"

namespace raytracer
{

    std::string PointLight::to_string() const
    {
        std::string s = "raytracer::PointLight\n";
        s += "    Color: " + m_color.to_string() + "\n";
        s += "    Position: " + m_position.to_string() + "\n";
        return s;
    }

}