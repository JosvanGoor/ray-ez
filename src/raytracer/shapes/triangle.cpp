#include "triangle.hpp"

namespace raytracer
{

    Hit Triangle::intersect(const Ray &ray)
    {
        Vector3d e1 = m_v1 - m_v0;
        Vector3d e2 = m_v2 - m_v0;
        Vector3d e1e2 = e1.cross(e2).normalized();

        Vector3d p = ray.direction().cross(e2);
        float a = e1.dot(p);
        if(a < 0.0001) return Hit::no_hit();

        float f = 1.0f / a;
        Vector3d s = ray.origin() - m_v0;
        float u = f * (s.dot(p));
        if(u < 0.0 || u > 1.0)
            return Hit::no_hit();

        Vector3d q = s.cross(e1);
        float v = f * (ray.direction().dot(q));
        if(v < 0.0 || u + v > 1.0)
            return Hit::no_hit();

        float t = f * (e2.dot(q));

        return Hit(this, t, e1e2);
    }

}