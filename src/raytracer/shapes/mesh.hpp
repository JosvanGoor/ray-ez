#ifndef RAYTRACER_SHAPES_MESH_HPP
#define RAYTRACER_SHAPES_MESH_HPP

#include "shape.hpp"
#include "triangle.hpp"
#include "../../lib/glm.hpp"

namespace raytracer
{

    class Mesh : public Shape
    {
    public:
        Mesh(const std::string &str, Material *mat, const Vector3d &pos, double scale);

        virtual Hit intersect(const Ray &ray);

        //todo tostring override
    
    protected:
        std::vector<Triangle> m_triangles;

        void read_simple_model(GLMmodel *model, const Vector3d &pos);
    };

}

#endif