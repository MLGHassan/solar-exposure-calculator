#include "Ray.hpp"

Ray::Ray(){
    this->o = Point3D(0, 0, 0);
    this->d = Vector3D(0, 0, 0);
    this->w = 1;
}

Ray::Ray(const Point3D &origin, const Vector3D &dir){

    this->o = origin;
    this->d = dir;
    this->w = 1;
}

std::string Ray::to_string() const {

   const std::string strret = this->o.to_string() + " " + this->d.to_string() + " " + std::to_string(this->w);
   std::cout << strret << std::endl;
   return strret;
    
}