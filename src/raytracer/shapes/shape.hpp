#ifndef RAYTRACER_SHAPES_SHAPE_HPP
#define RAYTRACER_SHAPES_SHAPE_HPP

#include "../hit.hpp"
#include "../ray.hpp"
#include "../../core.hpp"
#include "../material.hpp"

namespace raytracer
{

    /*
        Shape baseclass.
    */

    class Shape : public Object
    {
    public:
        Shape() : m_material(nullptr) {}
        virtual ~Shape() { if(m_material) { delete m_material; /*std::cout << "deleted material\n";*/ } }

        virtual Material* material() const;
        virtual void material(Material *mat);
        virtual Hit intersect(const Ray &ray) = 0;
        virtual Vector3d color_at(const Vector3d &point) const;
    
        //base override
        virtual std::string to_string() const;

    protected:
        Material *m_material;
    };

}

#endif
