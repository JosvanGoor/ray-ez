#include "hit.hpp"

#include <limits>
#include "shapes/shape.hpp"

namespace raytracer
{

    bool Hit::hit() const
    {
        return m_shape != nullptr;
    }

    bool Hit::missed() const
    {
        return m_shape == nullptr;
    }

    Hit Hit::no_hit()
    {
        static Hit rval(nullptr, std::numeric_limits<double>::infinity());
        return rval;
    }

    std::string Hit::to_string() const
    {
        std::string s = "raytracer::Hit\n";
        if(missed())
        {
            s += "    no hit.";
            return s;
        }
        s += "    distance: " + std::to_string(m_distance) + "\n";
        s += "    normal: " + m_normal.to_string() + "\n";
        s += "    shape: " + m_shape->to_string();
        return s;
    }

}