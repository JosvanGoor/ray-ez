#include <cstdio>
#include <cstdlib>
#include <iostream>

using namespace std;

#include "core.hpp"
#include "raytracer/shapes/sphere.hpp"
#include "data/image.hpp"
#include "raytracer/rendering/rendermodel.hpp"
#include "raytracer/rendering/phongshadingmodel.hpp"

using namespace raytracer;

int main(int argc, char **argv)
{
    Camera cam(Vector3d(0, 0.5, 0), Vector3d(200, 200, 1000), Vector3d(200, 200, 0));
    cam.set_image(800, 800, 2);

    Scene *scene = new Scene();
    scene->add_light(new PointLight(Vector3d(1.0), Vector3d(-200, 600, 1500)));
    Material *blue = new Material(Vector3d(0.0, 0.0, 0.14), Vector3d(0.0, 0.0, 0.7), Vector3d(0.5), 64);
    Material *green = new Material(Vector3d(0.0, 0.06, 0.0), Vector3d(0.0, 0.3, 0.0), Vector3d(0.5), 8);
    Material *red = new Material(Vector3d(0.14, 0.0, 0.0), Vector3d(0.7, 0.0, 0.0), Vector3d(0.5), 32);
    Material *yellow = new Material(Vector3d(0.16, 0.128, 0.0), Vector3d(0.8, 0.64, 0.0), Vector3d(0.0), 1);
    Material *orange = new Material(Vector3d(0.16, 0.08, 0.0), Vector3d(0.8, 0.4, 0.0), Vector3d(0.5), 32);
    Material *gray = new Material(Vector3d(0.08), Vector3d(0.32), Vector3d(0.75), 256);

    Sphere *sp = new Sphere(Vector3d(90, 320, 100), 50);
    sp->material(blue);
    scene->add_shape(sp);

    sp = new Sphere(Vector3d(210, 270, 300), 50);
    sp->material(green);
    scene->add_shape(sp);

    sp = new Sphere(Vector3d(290, 170, 150), 50);
    sp->material(red);
    scene->add_shape(sp);

    sp = new Sphere(Vector3d(140, 220, 400), 50);
    sp->material(yellow);
    scene->add_shape(sp);

    sp = new Sphere(Vector3d(110, 130, 200), 50);
    sp->material(orange);
    scene->add_shape(sp);

    sp = new Sphere(Vector3d(200, 200, -1000), 1000);
    sp->material(gray);
    scene->add_shape(sp);

    PhongShadingModel rm;
    rm.camera(cam);
    rm.scene(scene);
    rm.enable_shadows();
    rm.reflection_depth(8);

    std::cout << *scene << endl;
    data::Image *img = rm.render_threaded(8);
    img->write_to_file("image.png");
    
    delete img;
    delete scene;

    return 0;
}