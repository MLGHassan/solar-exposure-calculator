#include "BBox.hpp"

float max_element(float a, float b, float c){

    if (a >= b && a >= c) return a;
    if (b >= a && b >= c) return b;
    if (c >= a && c >= b) return c;
    return a;
}

float min_element(float a, float b, float c){

    if (a <= b && a <= c) return a;
    if (b <= a && b <= c) return b;
    if (c <= a && c <= b) return c;
    return a;
}

BBox::BBox(const Point3D& min, const Point3D& max){
    this->pmin = min;
    this->pmax = max;
}

std::string BBox::to_string() const {

    const std::string strret = this->pmin.to_string() + " " + this->pmax.to_string();
   std::cout << strret << std::endl;
   return strret;


}

bool BBox::hit(const Ray &ray, float &t_enter, float &t_exit) const{

    float txmin, tymin, tzmin, txmax, tymax, tzmax;

    //Checking for entering and exiting on each slab

    if (1/ray.d.x >= 0){
        txmin = (this->pmin.x - ray.o.x)*(1/ray.d.x);
        txmax = (this->pmax.x - ray.o.x)*(1/ray.d.x);
    }
    else{
        txmin = (this->pmax.x - ray.o.x)*(1/ray.d.x);
        txmax = (this->pmin.x - ray.o.x)*(1/ray.d.x);
    }

    //Checking if the ray is in the negatve region of y
    if (1/ray.d.y >= 0){
        tymin = (this->pmin.y - ray.o.y)*(1/ray.d.y);
        tymax = (this->pmax.y - ray.o.y)*(1/ray.d.y);
    }
    else{
        tymin = (this->pmax.y - ray.o.y)*(1/ray.d.y);
        tymax = (this->pmin.y - ray.o.y)*(1/ray.d.y);
    }

    //Checking if the ray is in the negatve region of z
    if (1/ray.d.z >= 0){
        tzmin = (this->pmin.z - ray.o.z)*(1/ray.d.z);
        tzmax = (this->pmax.z - ray.o.z)*(1/ray.d.z);
    }
    else{
        tzmin = (this->pmax.z - ray.o.z)*(1/ray.d.z);
        tzmax = (this->pmin.z - ray.o.z)*(1/ray.d.z);
    }

    //Checking if smallest entry point is less than largest exit point.
    if (max_element(txmin, tymin, tzmin) < min_element(txmax, tymax, tzmax) &&
        min_element(tzmax, tymax, tzmax) > kEpsilon){

            //If conditions are true, the ray hits bbox. Therefore, define entry and exit
            t_enter = max_element(txmin, tymin, tzmin);
            t_exit = min_element(txmax, tymax, tzmax);

            return true;
        }

    return false;


};

void BBox::extend(Geometry* g){};
void BBox::extend(const BBox& b){};

bool BBox::contains(const Point3D& p){

    //Check if point is within the bounds of the bbox
    if(this->pmin.x < p.x && this->pmin.y < p.y && this->pmin.z < p.z &&
        this->pmax.x > p.x && this->pmax.y > p.y && this->pmax.z > p.z)
        return true;
    return false;

};

bool BBox::overlaps(Geometry* g){

    //Create const bbox from geometry object
    const BBox& gbbox = g->getBBox();
    //Run BBox method overlap on self through abstraction
    return BBox::overlaps(gbbox);

};

bool BBox::overlaps(const BBox& b){
    
    if(pmax.x > b.pmin.x && pmax.y > b.pmin.y && pmax.z > b.pmin.z)
        return true;
    return false;

};

