#include "material.hpp"

#include <fstream>

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

    std::vector<ObjMaterial> parse_mtl_file(const std::string &file)
    {
        std::ifstream in(file);
        if(in.fail()) throw Exception(__PRETTY_FUNCTION__, "Failed to open material file " + file);

        ObjMaterial mat;

        while(!in.eof())
        {
            std::string buffer = get_line(in);
            if(trim(buffer) == "") continue;
            if(buffer.find_first_of("#") == 0) continue;

            //parse dem shizzle.


        }
    }
}