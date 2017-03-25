#include "mesh.hpp"

#include <limits>

namespace raytracer
{

    Mesh::Mesh(const std::string &str, Material *mat, const Vector3d &pos, double scale)
    {
        m_material = mat;

        GLMmodel *model = glmReadOBJ(str.c_str());
        glmUnitize(model);
        //normals
        glmScale(model, scale);

        std::cout << "Reading mesh\n" << std::endl;
        std::cout << "    numverts: " << model->numvertices << std::endl;
        std::cout << "    numtriangles: " << model->numtriangles << std::endl;
        m_triangles.reserve(model->numtriangles);

        read_simple_model(model, pos);
        glmDelete(model);
    }

    Hit Mesh::intersect(const Ray &ray)
    {
        Hit min_hit(nullptr, std::numeric_limits<double>::infinity());

        for(size_t i = 0; i < m_triangles.size(); ++i)
        {
            Hit hit = m_triangles[i].intersect(ray);
            if(hit.distance() < min_hit.distance())
            {
                min_hit = hit;
            }
        }

        //if(min_hit.missed()) return Hit::no_hit();
        return min_hit;
    }


    void Mesh::read_simple_model(GLMmodel *model, const Vector3d &pos)
    {
        for(size_t i = 0; i < model->numtriangles; ++i)
        {
            Vector3d v0(
                model->vertices[3 * model->triangles[i].vindices[0]+0] + pos.x(),
                model->vertices[3 * model->triangles[i].vindices[0]+1] + pos.y(),
                model->vertices[3 * model->triangles[i].vindices[0]+2] + pos.z());

            Vector3d v1(
                model->vertices[3 * model->triangles[i].vindices[1]+0] + pos.x(),
                model->vertices[3 * model->triangles[i].vindices[1]+1] + pos.y(),
                model->vertices[3 * model->triangles[i].vindices[1]+2] + pos.z());

            Vector3d v2(
                model->vertices[3 * model->triangles[i].vindices[2]+0] + pos.x(),
                model->vertices[3 * model->triangles[i].vindices[2]+1] + pos.y(),
                model->vertices[3 * model->triangles[i].vindices[2]+2] + pos.z());

            Triangle tri(v0, v1, v2);
            tri.material(m_material);
            m_triangles.push_back(tri);
        }
    }

}