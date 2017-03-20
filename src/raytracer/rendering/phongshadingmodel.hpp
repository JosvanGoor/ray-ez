#ifndef RAYTRACER_RENDERING_PHONGSHADINGMODEL_HPP
#define RAYTRACER_RENDERING_PHONGSHADINGMODEL_HPP

#include "rendermodel.hpp"

namespace raytracer
{

    class PhongShadingModel : public RenderModel
    {
    public:
        PhongShadingModel();
        virtual ~PhongShadingModel();

        //irtual data::Image* render();
        virtual Vector3d trace(const Ray &ray, size_t reflections_left);

        virtual std::string to_string() const;
    };

}

#endif