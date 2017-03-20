#ifndef RAYTRACER_MATERIAL_HPP
#define RAYTRACER_MATERIAL_HPP

#include <string>
#include "../core.hpp"

namespace raytracer
{

    class Material : public Object
    {
    public:
        Material(Vector3d am, Vector3d diff, Vector3d spec, double exp) 
            : m_ambient(am), m_diffuse(diff), m_specular(spec), m_specular_exponent(exp) { }

        virtual ~Material() { }

        virtual std::string to_string() const;

        const Vector3d m_ambient;
        const Vector3d m_diffuse;
        const Vector3d m_specular;
        const double m_specular_exponent;
    };

}

#endif