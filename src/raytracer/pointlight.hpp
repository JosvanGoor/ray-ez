#ifndef RAYTRACER_POINTLIGHT_HPP
#define RAYTRACER_POINTLIGHT_HPP

#include "../core.hpp"

namespace raytracer
{

    class PointLight : public Object
    {
    public:
        PointLight(const Vector3d &color, const Vector3d &position)
            : m_color(color), m_position(position) {}
        virtual ~PointLight() {};

        Vector3d color() const;
        Vector3d position() const;

        virtual std::string to_string() const;

    protected:
        const Vector3d m_color;
        const Vector3d m_position;
    };

}

#endif
