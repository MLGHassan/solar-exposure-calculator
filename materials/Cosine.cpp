#include "Cosine.hpp"
#include <cmath>

Cosine::Cosine()
{
    this->color = RGBColor();
}

Cosine::Cosine(float c){
    this->color = RGBColor(c);
}

Cosine::Cosine(float r, float g, float b)
{
    this->color = RGBColor(r, g, b);
}

Cosine::Cosine(const RGBColor &c){
    this->color = c;
}

Cosine::Cosine(const Cosine &other){
    this->color = other.color;
}

Cosine& Cosine::operator=(const Cosine &other){
    this->color = other.color;
    return *this;
}

/* Returned shade is: color * cos \theta.
 \theta is the angle between the normal at the hit pont and the ray.
 Assuming unit vectors, cos \theta = dot product of normal and -ray.dir.
*/
RGBColor Cosine::shade(const ShadeInfo &sinfo)const{
    Vector3D a = -sinfo.ray.d;
    a.normalize();
    Vector3D b = sinfo.normal;
    b.normalize();
    return this->color * (a * b);
}

