#include "shape.hpp"

#include "../shapes/shape.hpp"

namespace raytracer
{

    Vector3d Shape::color_at(const Vector3d &point) const
    {
        //std::cout << "shape returning diffuse: " << m_material->m_diffuse.to_string() << std::endl;
        return m_material->m_diffuse;
    }

    Material* Shape::material() const
    {
        return m_material;
    }

    void Shape::material(Material *mat)
    {
        m_material = mat;
    }

    std::string Shape::to_string() const
    {
        return "raytracer::Shape\n";
    }
}