#ifndef RAYTRACER_SCENE_HPP
#define RAYTRACER_SCENE_HPP

#include "hit.hpp"
#include "ray.hpp"
#include "pointlight.hpp"
#include "../core.hpp"
#include "shapes/shape.hpp"

namespace raytracer
{

    class Scene : public Object
    {
    public:
        virtual ~Scene(); //TODO: DEFINE

        Hit closest_hit(const Ray &ray) const;

        void add_shape(Shape *shape);
        void add_light(PointLight *light);

        const std::vector<Shape*>& shapes() const;
        const std::vector<PointLight*>& lights() const;

        virtual std::string to_string() const;

    protected:
        std::vector<Shape*> m_shapes;
        std::vector<PointLight*> m_lights;
    };

}

#endif
