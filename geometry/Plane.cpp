#include "Plane.hpp"
#include <string>

Plane::Plane(){
    a = Point3D(0, 0, 0);
    n = Vector3D(0, 1, 0);
}

Plane::Plane(const Point3D &pt, const Vector3D &n_)
{
    a = pt;
    n = n_;
    n.normalize();
}

Plane::Plane(const Plane &object){
    a = object.a;
    n = object.n;
}

Plane& Plane::operator=(const Plane &rhs){
    a = rhs.a;
    n = rhs.n;
    return *this;
}

std::string Plane::to_string() const{
    return "print plane not implemented";
}

//book listing 3.5
bool
Plane::hit(const Ray& ray, float& tmin, ShadeInfo& sr) const {
    double t = (a - ray.o) * n / (ray.d * n);
    if (t > kEpsilon) {
    tmin = t;
    sr.hit = true;
    sr.material_ptr = this->material_ptr;
    sr.normal = this->n;
    sr.ray = ray;
    sr.hit_point = ray.o + ray.d * tmin;
    sr.t = tmin;
    sr.hit_point = ray.o + t * ray.d;
    return (true);
    }
    else
    return (false);
}
BBox Plane::getBBox()const{
    return BBox();
}
