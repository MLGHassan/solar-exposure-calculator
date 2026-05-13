#include "Sphere.hpp"
#include <iostream>
#include <cmath>

Sphere::Sphere()
{
    c = Point3D(0, 0, 0);
    r = 0;
}

Sphere::Sphere(const Point3D &center, float radius)
{
    c = center;
    r = radius;
}

Sphere::Sphere(const Sphere &object)
{
    c = object.c;
    r = object.r;
}

Sphere &Sphere::operator=(const Sphere &rhs)
{
    c = rhs.c;
    r = rhs.r;
    return *this;
}

std::string Sphere::to_string() const
{
    return "print sphere not implemented";
}

// book listing 3.6
bool Sphere::hit(const Ray &ray, float &tmin, ShadeInfo &sr) const
{
    double t;
    Vector3D temp = ray.o - c;
    double a = ray.d * ray.d;
    double b = 2.0 * temp * ray.d;
    double c = temp * temp - r * r;
    double disc = b * b - 4.0 * a * c;
    if (disc < 0.0)
        return (false);
    else
    {
        double e = sqrt(disc);
        double denom = 2.0 * a;
        t = (-b - e) / denom; // smaller root
        if (t > kEpsilon)
        {
            tmin = t;
            sr.hit = true;
            sr.material_ptr = this->material_ptr;
            sr.ray = ray;
            sr.hit_point = ray.o + ray.d * tmin;
            sr.normal = sr.hit_point - this->c;
            sr.normal.normalize();
            sr.t = tmin;
            return (true);
        }
        t = (-b + e) / denom; // larger root
        if (t > kEpsilon)
        {
            tmin = t;
            sr.hit = true;
            sr.material_ptr = this->material_ptr;
            sr.ray = ray;
            sr.hit_point = ray.o + ray.d * tmin;
            sr.normal = sr.hit_point - this->c;
            sr.normal.normalize();
            sr.t = tmin;
            return (true);
        }
    }
    return (false);
}
BBox Sphere::getBBox() const
{   
    //point vector arithmetic gives a point 
    Point3D start = c - Vector3D(r);
    Point3D end = c + Vector3D(r);
    return BBox(start, end);
}
