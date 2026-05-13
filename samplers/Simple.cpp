#include "Simple.hpp"


Simple::Simple(Camera *c_ptr, ViewPlane *v_ptr)
{

    this->camera_ptr = c_ptr;
    this->viewplane_ptr = v_ptr;

}


Simple::Simple(const Simple &camera)
{

    this->camera_ptr = camera.camera_ptr;
    this->viewplane_ptr = camera.viewplane_ptr;

}


Simple & Simple::operator=(const Simple &other)
{

    this->camera_ptr = other.camera_ptr;
    this->viewplane_ptr = other.viewplane_ptr;
    return *this;

}

std::vector<Ray> Simple::get_rays(int px, int py) const
{

    float Lpx = px;
    float Lpy = py;

    Lpx += 0.5;
    Lpy += 0.5;

    //calculating viewPlane height and width
    float vpW = viewplane_ptr->bottom_right.x - viewplane_ptr->top_left.x;
    float vpH = viewplane_ptr->bottom_right.y - viewplane_ptr->top_left.y;

    //calculating pixel height and width
    float pixelH = vpH / viewplane_ptr->vres;
    float pixelW = vpW / viewplane_ptr->hres;

    Point3D point;

    point.x = Lpx * pixelW + viewplane_ptr->top_left.x;
    point.y = Lpy * pixelH + viewplane_ptr->top_left.y;
    point.z = viewplane_ptr->top_left.z;

    Vector3D dir = camera_ptr->get_direction(point);

    Ray r = Ray(point, dir);

    std::vector<Ray> rayVect = {r};

    return rayVect;
}


