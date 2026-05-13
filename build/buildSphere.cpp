#include "../cameras/Perspective.hpp"
#include "../geometry/Sphere.hpp"
#include "../materials/Cosine.hpp"
#include "../samplers/Simple.hpp"
#include "../utilities/Constants.hpp"
#include "../world/World.hpp"

void World::build(void) {
    // 1. View plane setup (the "canvas")
    vplane.top_left.x = -10;
    vplane.top_left.y = 10;
    vplane.top_left.z = 10;
    vplane.bottom_right.x = 10;
    vplane.bottom_right.y = -10;
    vplane.bottom_right.z = 10;
    vplane.hres = 400;
    vplane.vres = 400;

    // 2. Background color
    bg_color = black; 

    // 3. Camera and sampler
    // Position the camera at z=30, looking at the origin
    set_camera(new Perspective(0, 0, 30));
    sampler_ptr = new Simple(camera_ptr, &vplane);

    // 4. The Single Sphere
    // Place it exactly at the center (0,0,0) with a radius of 8
    Sphere* sphere_ptr = new Sphere(Point3D(0, 0, 0), 8);
    sphere_ptr->set_material(new Cosine(red)); 
    add_geometry(sphere_ptr);
}