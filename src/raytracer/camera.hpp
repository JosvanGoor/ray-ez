#ifndef RAYTRACER_CAMERA_HPP
#define RAYTRACER_CAMERA_HPP

#include "../core.hpp"

namespace raytracer
{

    class Camera : public Object
    {
    public:
        Camera()
            : m_image_width(400), m_image_height(400), m_supersamples(1),
              m_up(Vector3d(0.0, 1.0, 0.0)), m_eye(Vector3d(0.0, 1.0, 0.0)), m_center(Vector3d()), 
              m_depth_of_field(false), m_aperture_radius(0), m_aperture_samples(0) {};
        
        Camera(const Vector3d &eye)
            : m_image_width(400), m_image_height(400), m_supersamples(1),
              m_up(Vector3d(0.0, 1.0, 0.0)), m_eye(eye), m_center(Vector3d()), 
              m_depth_of_field(false), m_aperture_radius(0), m_aperture_samples(0) {};

        Camera(const Vector3d &up, const Vector3d &eye, const Vector3d &center)
            : m_image_width(400), m_image_height(400), m_supersamples(1),
              m_up(up), m_eye(eye), m_center(center), 
              m_depth_of_field(false), m_aperture_radius(0), m_aperture_samples(0) {};

        Camera(const Vector3d &up, const Vector3d &eye, const Vector3d &center, double dofrad, size_t samples)
            : m_image_width(400), m_image_height(400), m_supersamples(1),
              m_up(up), m_eye(eye), m_center(center), 
              m_depth_of_field(true), m_aperture_radius(dofrad), m_aperture_samples(samples) {};

        virtual std::string to_string() const;

        size_t m_image_width;
        size_t m_image_height;
        size_t m_supersamples;

        Vector3d m_up;
        Vector3d m_eye;
        Vector3d m_center;

        bool m_depth_of_field;
        double m_aperture_radius;
        size_t m_aperture_samples;
    };

}

#endif