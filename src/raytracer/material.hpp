#ifndef RAYTRACER_MATERIAL_HPP
#define RAYTRACER_MATERIAL_HPP

#include <string>
#include <vector>
#include "../core.hpp"

namespace raytracer
{

    //Material object parsed from mtl file.
    class ObjMaterial
    {
    public:
        ObjMaterial()
            : m_name(""),
            m_ambient(Vector3d()), m_diffuse(Vector3d()), m_specular(Vector3d()), m_specular_exponent(0),
            m_bump_map(""), m_displacement_map(""),
            m_ambient_texture(""), m_diffuse_texture(""), m_specular_texture("") { };

        std::string m_name; //name

        //material characteristicsc
        Vector3d m_ambient;
        Vector3d m_diffuse;
        Vector3d m_specular;
        double m_specular_exponent;

        //texture map filenames
        std::string m_bump_map;
        std::string m_displacement_map;

        std::string m_ambient_texture;
        std::string m_diffuse_texture;
        std::string m_specular_texture;
    };

    class Material : public Object
    {
    public:
        Material(Vector3d am, Vector3d diff, Vector3d spec, double exp) 
            : m_ambient(am), m_diffuse(diff), m_specular(spec), m_specular_exponent(exp) { }

        Material(const ObjMaterial &mat)
            : m_ambient(mat.m_ambient), m_diffuse(mat.m_diffuse), m_specular(mat.m_specular), 
              m_specular_exponent(mat.m_specular_exponent) { }
        virtual ~Material() { }

        virtual std::string to_string() const;

        const Vector3d m_ambient;
        const Vector3d m_diffuse;
        const Vector3d m_specular;
        const double m_specular_exponent;
    };

    std::vector<ObjMaterial> parse_mtl_file(const std::string &file);
}

#endif