#ifndef RAYTRACER_SHAPES_TRIANGLE_HPP
#define RAYTRACER_SHAPES_TRIANGLE_HPP

#include "shape.hpp"
#include "../../core.hpp"

namespace raytracer
{

    class Triangle : public Shape
    {
    public:
        Triangle(const Vector3d &v1, const Vector3d &v2, const Vector3d &v3)
            : m_v0(v1), m_v1(v2), m_v2(v3) { };

        virtual ~Triangle() { m_material = nullptr; } //release material before its deleted by shape

        virtual Hit intersect(const Ray &ray);    

        //TODO: override tostring
        //TODO: add smooth normals
        //TODO: add texture mapping

    protected:
        const Vector3d m_v0;
        const Vector3d m_v1;
        const Vector3d m_v2;
    };

}

#endif