#ifndef RAYTRACER_HIT_HPP
#define RAYTRACER_HIT_HPP

#include <string>
#include "../core.hpp"

namespace raytracer
{

    class Shape; //circular dependecy

    /*
        Represents a hitpoint, or miss when default constructed.
        contains information on the shape that was hit, the distance and the
        surface normal at the point of impact.
    */

    class Hit : public Object
    {
    public:
        Hit(Shape *shape = nullptr, double distance = 0, Vector3d normal = Vector3d())
            : m_shape(shape), m_distance(distance), m_normal(normal) { }

        Hit(const Hit &hit)
            : m_shape(hit.m_shape), m_distance(hit.m_distance), m_normal(hit.m_normal) { }

        //convenience functions
        bool hit() const;
        bool missed() const;

        Shape *m_shape;
        double m_distance;
        Vector3d m_normal;

        static Hit no_hit();

        virtual std::string to_string() const;
    };

}

#endif