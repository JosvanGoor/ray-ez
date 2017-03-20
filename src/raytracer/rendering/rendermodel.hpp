#ifndef RAYTRACER_RENDERING_RENDERMODEL_HPP
#define RAYTRACER_RENDERING_RENDERMODEL_HPP

#include "../hit.hpp"
#include "../ray.hpp"
#include "../scene.hpp"
#include "../camera.hpp"
#include "../material.hpp"
#include "../pointlight.hpp"
#include "../../core.hpp"
#include "../shapes/shape.hpp"
#include "../../data/image.hpp"

namespace raytracer
{

    class RenderModel : public Object
    {
    public:
        RenderModel();
        virtual ~RenderModel();

        virtual data::Image* render();
        virtual Vector3d trace(const Ray &ray, size_t reflections_left);

        /*
            Setters & Getters
        */
        bool shadows() const;
        void enable_shadows();
        void disable_shadows();

        Scene* scene();
        void scene(Scene *s);

        Camera camera() const;
        void camera(const Camera &cam);

        size_t reflection_depth() const;
        void reflection_depth(size_t rd);

        virtual std::string to_string() const; //lekker later

    protected:
        bool m_shadows;
        size_t m_reflection_depth;
        Vector3d m_background_color;

        //default render types.
        virtual void render_simple();
        virtual void render_with_supersampling();
        virtual void render_with_dof_and_supersampling();

        Scene *m_scene;
        Camera m_camera;

        //values used during all renderstages.
        double pixel_size;
        size_t img_w, img_h;
        data::Image *image;
        Vector3d G, A, B, H, V, origin, offset_h, offset_v;
    };

}

#endif