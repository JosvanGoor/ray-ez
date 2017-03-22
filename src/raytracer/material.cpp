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
        using std::stof;
        std::ifstream in(file);
        if(in.fail()) throw Exception(__PRETTY_FUNCTION__, "Failed to open material file " + file);

        ObjMaterial mat;
        std::vector<ObjMaterial> materials;

        while(!in.eof())
        {
            std::string buffer = get_line(in);
            if(buffer.find_first_of("#") == 0) continue;

            //parse dem shizzle.
            std::vector<std::string> parts = split(trim(buffer), ' ');
            if(parts.size() < 2) continue; //not relevant
            
            //new material
            if(parts[0] == "newmtl")
            {
                //if made push current
                if(mat.m_name != "") materials.push_back(mat);
                mat = ObjMaterial();
                mat.m_name = parts[1];
            }
            //basic components
            else if(parts[0] == "Ka")
                mat.m_ambient = Vector3d(stof(parts[1]), stof(parts[2]), stof(parts[3]));
            else if(parts[0] == "Kd")
                mat.m_diffuse = Vector3d(stof(parts[1]), stof(parts[2]), stof(parts[3]));
            else if(parts[0] == "Ks")
                mat.m_specular = Vector3d(stof(parts[1]), stof(parts[2]), stof(parts[3]));
            else if(parts[0] == "Ns")
                mat.m_specular_exponent = stof(parts[1]);
            
            //texture maps
            else if(parts[0] == "map_Ka")
                mat.m_ambient_texture = parts[1];
            else if(parts[0] == "map_Kd")
                mat.m_diffuse_texture = parts[1];
            else if(parts[0] == "map_Ks")
                mat.m_specular_texture = parts[1];
            else if(parts[0] == "bump" || parts[0] == "map_bump")
                mat.m_bump_map = parts[1];
            else if(parts[0] == "disp")
                mat.m_displacement_map = parts[1];
        }

        return materials;
    }
}