#include "scene.hpp"

#include <limits>
#include "hit.hpp"

namespace raytracer
{

    Scene::~Scene()
    {
        for(Shape *sh : m_shapes)
            delete sh;
        m_shapes.clear();

        for(PointLight *pl : m_lights)
            delete pl;
        m_lights.clear();
    }

    Hit Scene::closest_hit(const Ray &ray) const
    {
        Hit min_hit(nullptr, std::numeric_limits<double>::infinity());

        for(size_t i = 0; i < m_shapes.size(); ++i)
        {
            Hit hit = m_shapes[i]->intersect(ray);
            if(hit.m_distance < min_hit.m_distance)
            {
                min_hit = hit;
            }
        }

        //if(min_hit.missed()) return Hit::no_hit();
        return min_hit;
    }

    void Scene::add_shape(Shape *shape) { m_shapes.push_back(shape); }
    void Scene::add_light(PointLight *light) { m_lights.push_back(light); }
    const std::vector<Shape*>& Scene::shapes() const { return m_shapes; }
    const std::vector<PointLight*>& Scene::lights() const { return m_lights; }

    std::string Scene::to_string() const
    {
        std::string s = "raytracer::Scene\n";
        s += "    Objects: " + std::to_string(m_shapes.size()) + "\n";
        s += "    Lights: " + std::to_string(m_lights.size()) + "\n";
        return s;
    }

}