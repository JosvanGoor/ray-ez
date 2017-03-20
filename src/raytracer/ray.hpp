#ifndef RAYTRACER_RAY_HPP
#define RAYTRACER_RAY_HPP

#include <string>
#include "../core.hpp"

namespace raytracer
{

    class Ray : public Object
    {
    public:
        Ray(const Vector3d &origin, const Vector3d &direction)
            : m_origin(origin), m_direction(direction) { };

        Vector3d at(double distance) const;
        virtual std::string to_string() const;

        const Vector3d m_origin;
        const Vector3d m_direction;
    };

}

#endif