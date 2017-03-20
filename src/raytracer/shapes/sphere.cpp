#include "sphere.hpp"

namespace raytracer
{

    Hit Sphere::intersect(const Ray &ray)
    {
        Vector3d oc = ray.m_origin - m_center;

        double a = ray.m_direction.dot(ray.m_direction);
        double b = 2 * oc.dot(ray.m_direction);
        double c = oc.dot(oc) - (m_radius * m_radius);
        double disc = b * b - 4 * a * c;
        double t;

        if(disc < 0) return Hit::no_hit();
        else
        {
            disc = sqrt(disc);
            double t1 = (-b - disc) / (2 * a);
            double t2 = (-b + disc) / (2 * a);

            if(t1 < 0) t = t2;
            else if(t2 < 0) t = t1;
            else if(t1 < t2) t = t1;
            else t = t2;
        }

        if(t < 0.001) return Hit::no_hit();

        Vector3d intersect = ray.at(t);
        Vector3d N = (intersect - m_center) / m_radius;
        if(ray.m_direction.dot(N) > 0) N = -N;

        return Hit(this, t, N);
    }

}