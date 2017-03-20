#include "material.hpp"

namespace raytracer
{

    std::string Material::to_string() const
    {
        std::string s = "raytracer::Material\n";
        s += "    ambient: " + m_ambient.to_string() + "\n";
        s += "    diffuse: " + m_diffuse.to_string() + "\n";
        s += "    specular: " + m_specular.to_string() + " - " + std::to_string(m_specular_exponent) + "\n";
        return s;
    }

}