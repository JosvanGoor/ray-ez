#include "rendermodel.hpp"

#include <chrono>

namespace raytracer
{

    RenderModel::RenderModel()
    {
        m_shadows = false;
        m_reflection_depth = 0;
        m_background_color = Vector3d(0.0);

        m_scene = nullptr;
        m_camera = Camera(Vector3d(1.0, 0.0, 0.0));
    }

    RenderModel::~RenderModel()
    {
        //do not delete scene, it can be shared with multiple models.
    }

    data::Image* RenderModel::render()
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
        if(m_camera.m_supersamples == 0) render_simple();
        else if(m_camera.m_depth_of_field) render_with_dof_and_supersampling();
        else render_with_supersampling();
        double elapsed = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - current_time).count();

        std::cout << "render completed in " << (elapsed / 1000) << " seconds." << std::endl;

        return image;
    }

    Vector3d RenderModel::trace(const Ray &ray, size_t reflections)
    {
        Hit min_hit = m_scene->closest_hit(ray);

        if(min_hit.hit()) return min_hit.m_shape->color_at(ray.at(min_hit.m_distance));
        return Vector3d();
    }

    bool RenderModel::shadows() const { return m_shadows; }
    void RenderModel::enable_shadows() { m_shadows = true; }
    void RenderModel::disable_shadows() { m_shadows = false; }
    Scene* RenderModel::scene() { return m_scene; }
    void RenderModel::scene(Scene *scene) { m_scene = scene; }
    Camera RenderModel::camera() const { return m_camera; }
    void RenderModel::camera(const Camera &cam) { m_camera = cam; }
    size_t RenderModel::reflection_depth() const { return m_reflection_depth; }
    void RenderModel::reflection_depth(size_t rd) { m_reflection_depth = rd; }

    std::string RenderModel::to_string() const
    {
        return "raytracer::RenderModel";
    }

    ///////////////////////////////////////////////////////////////////////////////////////////////////
    // Big render functions
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    void RenderModel::render_simple()
    {
        for(size_t x = 0; x < img_w; ++x)
            for(size_t y = 0; y < img_h; ++y)
            {
                Vector3d pixel(x + 0.5, img_h - 1 - y - 0.6, 0);
                Ray ray(m_camera.m_eye, (pixel - m_camera.m_eye).normalized());
                Vector3d color = trace(ray, 0);

                image->set_pixel(color, x, y);
            }
    }

    void RenderModel::render_with_supersampling()
    {   
        for(size_t y = 0; y < img_h; ++y)
        {
            for(size_t x = 0; x < img_w; ++x)
            {
                Vector3d average(0.0);
                Vector3d pixel = origin + x * H + (img_h - pixel_size - y) * V;

                for(size_t i = 0; i < m_camera.m_supersamples; ++i)
                {
                    for(size_t j = 0; j < m_camera.m_supersamples; ++j)
                    {
                        Vector3d des = pixel + (i * offset_h) + (j * offset_v);
                        des = des + (offset_h / 2) + (offset_v / 2);
                        Ray ray(m_camera.m_eye, (des - m_camera.m_eye).normalized());
                        average += trace(ray, m_reflection_depth);
                    }
                }
                average /= (m_camera.m_supersamples * m_camera.m_supersamples);
                image->set_pixel(average, x, y);
            }
        }
    }

    void RenderModel::render_with_dof_and_supersampling()
    {
        for(size_t y = 0; y < img_h; ++y)
        {
            for(size_t x = 0; x < img_w; ++x)
            {
                Vector3d average(0.0);
                Vector3d pixel = origin + x * H + (img_h - pixel_size - y) * V;

                double c = m_camera.m_aperture_radius / (m_camera.m_up.length() * sqrt(m_camera.m_aperture_samples));

                //loop through dof angles
                for(size_t dof = 0; dof < m_camera.m_aperture_samples; ++dof)
                {
                    double r = c * sqrt(dof);
                    //last part = golden angle
                    double theta = dof * (180.0 * (3.0 - sqrt(5.0)));
                    Vector3d dofeye = m_camera.m_eye;

                    dofeye += (r * A * cos(theta)); //y displacement
                    dofeye += (r * m_camera.m_up * sin(theta)); //x displacement

                    //supersample dof angles
                    for(size_t i = 0; i < m_camera.m_supersamples; ++i)
                    {
                        for(size_t j = 0; j < m_camera.m_supersamples; ++j)
                        {
                            Vector3d des = pixel + (i * offset_h) + (j * offset_v);
                            des = des + (offset_h / 2) + (offset_v / 2);
                            Ray ray(dofeye, (des - dofeye).normalized());
                            average += trace(ray, m_reflection_depth);
                        }
                    }
                }
                average /= (m_camera.m_supersamples * m_camera.m_supersamples) * m_camera.m_aperture_samples;
                image->set_pixel(average, x, y);
            }
        }
    }
}