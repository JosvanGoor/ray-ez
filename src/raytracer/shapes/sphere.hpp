#ifndef RAYTRACER_SHAPES_SPHERE_HPP
#define RAYTRACER_SHAPES_SPHERE_HPP

#include "shape.hpp"

namespace raytracer
{

    class Sphere : public Shape
    {
    public:
        Sphere(const Vector3d &point, double radius)
            : m_center(point), m_radius(radius) { }
        
        virtual ~Sphere() { };

        virtual Hit intersect(const Ray &ray);

        //TODO: override tostring
    
    protected:
        const Vector3d m_center;
        const double m_radius;

    };

}

#endif