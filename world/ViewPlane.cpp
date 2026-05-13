#include "ViewPlane.hpp"


ViewPlane::ViewPlane()
{
    top_left = Point3D(-320, 240, 0);
    bottom_right = Point3D(top_left.x + hres, top_left.y + vres,0);
    normal = Vector3D(0, 0, -1);
    hres = 640;
    vres = 480;
}
//other constructors and destructor are default so don't need to declare those

int ViewPlane::get_hres() const
{
    return hres;
}


void ViewPlane::set_hres(int hresVal)
{
    hres = hresVal;
}


int ViewPlane::get_vres() const
{
    return vres;
}


void ViewPlane::set_vres(int vresVal)
{
    vres = vresVal;
}
