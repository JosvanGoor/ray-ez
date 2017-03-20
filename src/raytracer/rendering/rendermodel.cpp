#include "rendermodel.hpp"

#include <chrono>
#include <thread>

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
        
        image = new data::Image(m_camera.image_width(), m_camera.image_height());

        img_w = m_camera.image_width();
        img_h = m_camera.image_height();

        pixel_size = m_camera.up().length();
        G = (m_camera.center() - m_camera.eye()).normalized();
        A = (G.cross(m_camera.up())).normalized();
        B = (A.cross(G)).normalized();

        H = pixel_size * A;
        V = pixel_size * B;
        origin = m_camera.center()
            - ((m_camera.image_width() / 2) * H)
            - ((m_camera.image_height() / 2) * V);

        offset_h = H / m_camera.supersamples();
        offset_v = V / m_camera.supersamples();

        
        auto current_time = std::chrono::high_resolution_clock::now();
        if(m_camera.supersamples() == 0) render_simple();
        else if(m_camera.depth_of_field()) render_with_dof_and_supersampling();
        else render_with_supersampling();
        double elapsed = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - current_time).count();

        std::cout << "\rrender completed in " << (elapsed / 1000) << " seconds." << std::endl;

        return image;
    }

    Vector3d RenderModel::trace(const Ray &ray, size_t reflections)
    {
        Hit min_hit = m_scene->closest_hit(ray);

        if(min_hit.hit()) return min_hit.shape()->color_at(ray.at(min_hit.distance()));
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
    // Threaded functions (public & private)
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    void worker(RenderModel *model)
    {
        std::vector<Vector3d> storage;

        int y = model->get_work(0);
        while(y != -1)
        {
            storage.clear();
            storage.reserve(model->img_w);

            if(model->m_camera.supersamples() == 0) model->render_simple_threaded(y);
            else if(model->m_camera.depth_of_field()) model->render_with_dof_and_supersampling_threaded(y);
            else model->render_with_supersampling_threaded(y);

            y = model->get_work(y);
        }
    }

    int RenderModel::get_work(int y)
    {
        thread_lock.lock(); //synchronize

        //assign next line
        int rval = current < img_h ? current : -1;
        ++current;

        //update console?
        float progress = ((float)current / (float)(img_h - 1.0)) * 100;
        size_t pr = floor(progress);
        if(pr != reported)
        {
            std::cout << "\rProgress: " << pr << "%" << std::flush;
            reported = pr;
        }

        thread_lock.unlock(); //unsync
        return rval; //return next task.
    }

    data::Image* RenderModel::render_threaded(size_t thread_count)
    {
        if(!m_scene) throw Exception(__PRETTY_FUNCTION__, "no scene set");   
        image = new data::Image(m_camera.image_width(), m_camera.image_height());

        img_w = m_camera.image_width();
        img_h = m_camera.image_height();

        pixel_size = m_camera.up().length();
        G = (m_camera.center() - m_camera.eye()).normalized();
        A = (G.cross(m_camera.up())).normalized();
        B = (A.cross(G)).normalized();

        H = pixel_size * A;
        V = pixel_size * B;
        origin = m_camera.center()
            - ((m_camera.image_width() / 2) * H)
            - ((m_camera.image_height() / 2) * V);

        offset_h = H / m_camera.supersamples();
        offset_v = V / m_camera.supersamples();

        auto current_time = std::chrono::high_resolution_clock::now();
        current = 0;
        reported = 0;
        std::vector<std::thread> threads;
        threads.reserve(thread_count - 1);
        
        //spawn the threads
        for(size_t i = 0; i < thread_count - 1; ++i)
        {
            threads.push_back(std::thread(worker, this));
        }

        //when subthreads are spawned start working ourself
        worker(this);

        //when were done wait for other threads to complete
        for(size_t i = 0; i < thread_count - 1; ++i)
        {
            threads[i].join();
        }

        //all threads are done so the image must be aswell.

        double elapsed = std::chrono::duration<double, std::milli>(std::chrono::high_resolution_clock::now() - current_time).count();
        std::cout << "\rrender completed in " << (elapsed / 1000) << " seconds on " << thread_count << " threads." << std::endl;
        return image;
    }

    void RenderModel::render_simple_threaded(int y)
    {
        for(size_t x = 0; x < img_w; ++x)
        {
            Vector3d pixel(x + 0.5, img_h - 1 - y - 0.6, 0);
            Ray ray(m_camera.eye(), (pixel - m_camera.eye()).normalized());
            Vector3d color = trace(ray, 0);

            image->set_pixel(color, x, y);
        }
    }

    void RenderModel::render_with_supersampling_threaded(int y)
    {
        for(size_t x = 0; x < img_w; ++x)
        {
            Vector3d average(0.0);
            Vector3d pixel = origin + x * H + (img_h - pixel_size - y) * V;

            for(size_t i = 0; i < m_camera.supersamples(); ++i)
            {
                for(size_t j = 0; j < m_camera.supersamples(); ++j)
                {
                    Vector3d des = pixel + (i * offset_h) + (j * offset_v);
                    des = des + (offset_h / 2) + (offset_v / 2);
                    Ray ray(m_camera.eye(), (des - m_camera.eye()).normalized());
                    average += trace(ray, m_reflection_depth);
                }
            }
            average /= (m_camera.supersamples() * m_camera.supersamples());
            image->set_pixel(average, x, y);
        }
    }

    void RenderModel::render_with_dof_and_supersampling_threaded(int y)
    {
        for(size_t x = 0; x < img_w; ++x)
        {
            Vector3d average(0.0);
            Vector3d pixel = origin + x * H + (img_h - pixel_size - y) * V;

            double c = m_camera.aperture_radius() / (m_camera.up().length() * sqrt(m_camera.aperture_samples()));

            //loop through dof angles
            for(size_t dof = 0; dof < m_camera.aperture_samples(); ++dof)
            {
                double r = c * sqrt(dof);
                //last part = golden angle
                double theta = dof * (180.0 * (3.0 - sqrt(5.0)));
                Vector3d dofeye = m_camera.eye();

                dofeye += (r * A * cos(theta)); //y displacement
                dofeye += (r * m_camera.up() * sin(theta)); //x displacement

                //supersample dof angles
                for(size_t i = 0; i < m_camera.supersamples(); ++i)
                {
                    for(size_t j = 0; j < m_camera.supersamples(); ++j)
                    {
                        Vector3d des = pixel + (i * offset_h) + (j * offset_v);
                        des = des + (offset_h / 2) + (offset_v / 2);
                        Ray ray(dofeye, (des - dofeye).normalized());
                        average += trace(ray, m_reflection_depth);
                    }
                }
            }
            average /= (m_camera.supersamples() * m_camera.supersamples()) * m_camera.aperture_samples();
            image->set_pixel(average, x, y);
        }
    }


    ///////////////////////////////////////////////////////////////////////////////////////////////////
    // Big render functions
    ///////////////////////////////////////////////////////////////////////////////////////////////////
    void RenderModel::render_simple()
    {
        for(size_t y = 0; y < img_h; ++y)
            render_simple_threaded(y);
    }

    void RenderModel::render_with_supersampling()
    {
        for(size_t y = 0; y < img_h; ++y)
            render_with_supersampling_threaded(y);
    }

    void RenderModel::render_with_dof_and_supersampling()
    {
        for(size_t y = 0; y < img_h; ++y)
            render_with_dof_and_supersampling_threaded(y);
    }
    
    /*
    void RenderModel::render_simple()
    {
        for(size_t x = 0; x < img_w; ++x)
            for(size_t y = 0; y < img_h; ++y)
            {
                Vector3d pixel(x + 0.5, img_h - 1 - y - 0.6, 0);
                Ray ray(m_camera.eye(), (pixel - m_camera.eye()).normalized());
                Vector3d color = trace(ray, 0);

                image->set_pixel(color, x, y);
            }
    }

    void RenderModel::render_with_supersampling()
    {   
        for(size_t y = 0; y < img_h; ++y)
        {
            std::cout << "\rAt line " << y + 1 << "/" << img_h << std::flush;
            for(size_t x = 0; x < img_w; ++x)
            {
                Vector3d average(0.0);
                Vector3d pixel = origin + x * H + (img_h - pixel_size - y) * V;

                for(size_t i = 0; i < m_camera.supersamples(); ++i)
                {
                    for(size_t j = 0; j < m_camera.supersamples(); ++j)
                    {
                        Vector3d des = pixel + (i * offset_h) + (j * offset_v);
                        des = des + (offset_h / 2) + (offset_v / 2);
                        Ray ray(m_camera.eye(), (des - m_camera.eye()).normalized());
                        average += trace(ray, m_reflection_depth);
                    }
                }
                average /= (m_camera.supersamples() * m_camera.supersamples());
                image->set_pixel(average, x, y);
            }
        }
    }

    void RenderModel::render_with_dof_and_supersampling()
    {
        for(size_t y = 0; y < img_h; ++y)
        {
            std::cout << "\rAt line " << y + 1 << "/" << img_h << std::flush;
            for(size_t x = 0; x < img_w; ++x)
            {
                Vector3d average(0.0);
                Vector3d pixel = origin + x * H + (img_h - pixel_size - y) * V;

                double c = m_camera.aperture_radius() / (m_camera.up().length() * sqrt(m_camera.aperture_samples()));

                //loop through dof angles
                for(size_t dof = 0; dof < m_camera.aperture_samples(); ++dof)
                {
                    double r = c * sqrt(dof);
                    //last part = golden angle
                    double theta = dof * (180.0 * (3.0 - sqrt(5.0)));
                    Vector3d dofeye = m_camera.eye();

                    dofeye += (r * A * cos(theta)); //y displacement
                    dofeye += (r * m_camera.up() * sin(theta)); //x displacement

                    //supersample dof angles
                    for(size_t i = 0; i < m_camera.supersamples(); ++i)
                    {
                        for(size_t j = 0; j < m_camera.supersamples(); ++j)
                        {
                            Vector3d des = pixel + (i * offset_h) + (j * offset_v);
                            des = des + (offset_h / 2) + (offset_v / 2);
                            Ray ray(dofeye, (des - dofeye).normalized());
                            average += trace(ray, m_reflection_depth);
                        }
                    }
                }
                average /= (m_camera.supersamples() * m_camera.supersamples()) * m_camera.aperture_samples();
                image->set_pixel(average, x, y);
            }
        }
    }
    */
}