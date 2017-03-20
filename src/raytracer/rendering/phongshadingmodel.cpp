#include "phongshadingmodel.hpp"

#include <cmath>
#include <algorithm>

using std::max;

namespace raytracer
{

    PhongShadingModel::PhongShadingModel() : RenderModel() { }

    PhongShadingModel::~PhongShadingModel() { }

    /*data::Image* PhongShadingModel::render()
    {
        if(!m_scene) throw Exception(__PRETTY_FUNCTION__, "no scene set");
        
        image = new data::Image(m_camera.m_image_width, m_camera.m_image_height);

        img_w = m_camera.m_image_width;
        img_h = m_camera.m_image_height;

        pixel_size = m_camera.m_up.length();
        G = (m_camera.m_center - m_camera.m_eye).normalized();
        A = (G.cross(m_camera.m_up)).normalized();
        B = (A.cross(G)).normalized();

        H = pixel_size * A;
        V = pixel_size * B;
        origin = m_camera.m_center
            - ((m_camera.m_image_width / 2) * H)
            - ((m_camera.m_image_height / 2) * V);

        offset_h = H / m_camera.m_supersamples;
        offset_v = V / m_camera.m_supersamples;

        
        auto current_time = std::chrono::high_resolution_clock::now();
        if(m_camera.m_depth_of_field) render_with_dof_and_supersampling();
        else render_with_supersampling();
        double elapsed = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - current_time).count();

        std::cout << "render completed in " << (elapsed / 1000) << " seconds." << std::endl;

        return image;
    }*/

    Vector3d PhongShadingModel::trace(const Ray &ray, size_t reflections_left)
    {
        Hit min_hit = m_scene->closest_hit(ray);
        if(min_hit.missed()) return m_background_color;
        static Vector3d black = Vector3d(0.0);

        Vector3d hit = ray.at(min_hit.distance());
        Vector3d color;

        color = min_hit.shape()->material()->m_ambient;

        //for all lights
        for(size_t i = 0; i < m_scene->lights().size(); ++i)
        {
            Vector3d L = (m_scene->lights()[i]->position() - hit).normalized();
            Vector3d R = (2.0 * L.dot(min_hit.normal()) * min_hit.normal() - L).normalized();

            //sharp shadows
            if(m_shadows && m_scene->closest_hit(Ray(m_scene->lights()[i]->position(), -L)).shape() != min_hit.shape()) continue;

            color += max(0.0, L.dot(min_hit.normal())) * min_hit.shape()->color_at(hit) * m_scene->lights()[i]->color();
            color += pow(max(0.0, R.dot(-ray.direction())), min_hit.shape()->material()->m_specular_exponent) * min_hit.shape()->material()->m_specular * m_scene->lights()[i]->color();
        }

        if(reflections_left != 0)
        {
            if(min_hit.shape()->material()->m_specular != black)
            {
                Vector3d R = ray.direction().reflect_over(min_hit.normal());
                color += trace(Ray(hit, R), reflections_left - 1) * min_hit.shape()->material()->m_specular;
            }
        }
        color.clamp();
        return color;
    }

    std::string PhongShadingModel::to_string() const
    {
        return "PhongShadingModel doe tostring nog eem dan";
    }

}