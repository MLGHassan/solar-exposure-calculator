#include <iostream>
#include <cmath>
#include "Point3D.hpp"


Point3D::Point3D(){

   this->x = 0;
   this->y = 0;
   this->z = 0;
}
Point3D::Point3D(float c){
   this->x = c;
   this->y = c;
   this->z = c;
}
Point3D::Point3D(float _x, float _y, float _z){
   this->x = _x;
   this->y = _y;
   this->z = _z;
}


std::string Point3D::to_string() const{

   const std::string strret = std::to_string(this->x) + " " + std::to_string(this->y) + " " + std::to_string(this->z);
   std::cout << strret << std::endl;

   return strret;

}

//Arithmetic
Point3D Point3D::operator-() const {
   return Point3D(-this->x, -this->y, -this->z);
}

Vector3D Point3D::operator-(const Point3D& p) const {
   return Vector3D(this->x - p.x, this->y - p.y, this->z - p.z );
}
Point3D Point3D::operator+(const Vector3D& v) const {
   return Point3D(this->x+v.x, this->y+v.y, this->z+v.z);
}

Point3D Point3D::operator-(const Vector3D& v) const {
   return Point3D(this->x - v.x, this->y - v.y, this->z - v.z);
}

Point3D Point3D::operator*(const float s) const {
   return Point3D(this->x*s, this->y*s, this->z*s);
}

float Point3D::d_squared(const Point3D& p) const
{
    return pow((x - p.x),2) + pow((y - p.y),2) + pow((z - p.z),2);
}

float Point3D::distance(const Point3D& p) const
{
    return sqrt(Point3D::d_squared(p));
}


Point3D operator*(const float a, const Point3D& pt)
{
    return Point3D(a*pt.x, a*pt.y, a*pt.z);
}

Point3D min(const Point3D& a, const Point3D& b)
{
    return Point3D(std::min(a.x, b.x), std::min(a.y, b.y), std::min(a.z, b.z));
}

Point3D max(const Point3D& a, const Point3D& b)
{
    return Point3D(std::max(a.x, b.x), std::max(a.y, b.y), std::max(a.z, b.z));
}
