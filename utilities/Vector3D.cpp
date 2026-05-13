#include <cmath>
#include "Vector3D.hpp"

Vector3D::Vector3D(){

   x = 0;
   y = 0;
   z = 0;
}

Vector3D::Vector3D(double c){

   x = c;
   y = c;
   z = c;
}
Vector3D::Vector3D(double _x, double _y, double _z){

   x = _x;
   y = _y;
   z = _z;

}

std::string Vector3D::to_string() const{

   const std::string strret = std::to_string(this->x) + " " + std::to_string(this->y) + " " + std::to_string(this->z);
   std::cout << strret << std::endl;
   return strret;

}

Vector3D Vector3D::operator-() const {
   return Vector3D(-this->x, -this->y, -this->z);
}

Vector3D Vector3D::operator+(const Vector3D& v) const {
   return Vector3D(this->x+v.x, this->y+v.y, this->z+v.z);
}

Vector3D& Vector3D::operator+=(const Vector3D& v){

   this->x += v.x;
   this->y += v.y;
   this->z += v.z;
   return (*this);

}

Vector3D Vector3D::operator-(const Vector3D& v) const {
   return Vector3D(this->x - v.x, this->y - v.y, this->z - v.z);
}

Vector3D Vector3D::operator-=(const Vector3D& v) const{

   double x, y, z;
   x = this->x - v.x;
   y = this->y - v.y;
   z = this->z - v.z;
   return Vector3D(x, y, z);

}

Vector3D Vector3D::operator*(const double a) const {
   return Vector3D(this->x*a, this->y*a, this->z*a);
}

Vector3D Vector3D::operator/(const double a) const {
   return Vector3D(this->x/a, this->y/a, this->z/a);
}

void Vector3D::normalize()
{
    double len = Vector3D::length();
    this->x /= len;
    this->y /= len;
    this->z /= len;
}

double Vector3D::length() const
{
    return sqrt(Vector3D::len_squared());
}

double Vector3D::len_squared() const
{
    return (pow(this->x,2) + pow(this->y,2) + pow(this->z,2));
}

double Vector3D::operator*(const Vector3D& b) const
{
    return (this->x*b.x + this->y*b.y + this->z*b.z);
}

Vector3D Vector3D::operator^(const Vector3D& v) const
{
    return Vector3D((this->y*v.z)-(this->z*v.y), (this->z*v.x)-(this->x*v.z), (this->x*v.y)-(this->y*v.x));
}

Vector3D operator*(const double a, const Vector3D& v)
{
    return Vector3D(a*v.x, a*v.y, a*v.z);
}
