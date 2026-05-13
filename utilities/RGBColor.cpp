#include "RGBColor.hpp"
#include <cmath>

RGBColor::RGBColor(){

  this->r = 0;
   this->g = 0;
   this->b = 0;

};                             // set color to (0, 0, 0).
RGBColor::RGBColor(float c){
    this->r = c;
   this->g = c;
   this->b = c;
}

                    // set color to (c, c, c).
RGBColor::RGBColor(float _r, float _g, float _b){
  this->r = _r;
   this->g = _g;
   this->b = _b;


}; // set color to (_r, _g, _b).

std::string RGBColor::to_string() const{

   const std::string strret = std::to_string(this->r) + " " + std::to_string(this->g) + " " + std::to_string(this->b);
   std::cout << strret << std::endl;

   return strret;

}

RGBColor RGBColor::operator+(const RGBColor &c) const{

   return RGBColor(this->r+c.r, this->g+c.g, this->b+c.b);
}

RGBColor& RGBColor::operator+=(const RGBColor &c){

   this->r += c.r;
   this->g += c.g;
   this->b += c.b;
   return *this;

};

RGBColor RGBColor::operator*(const float a) const{
    return RGBColor(this->r*a, this->g*a, this->b*a);
}

RGBColor& RGBColor::operator*=(const float a){

   this->r *= a;
   this->g *= a;
   this->b *= a;
   return *this;

};

RGBColor RGBColor::operator/(const float a) const{
    return RGBColor(this->r/a, this->g/a, this->b/a);
}

RGBColor& RGBColor::operator/=(const float a){

   this->r /= a;
   this->g /= a;
   this->b /= a;
   return *this;

}
RGBColor RGBColor::operator*(const RGBColor &c) const{
    return RGBColor(this->r*c.r, this->g*c.g, this->b*c.b);
}

bool RGBColor::operator==(const RGBColor &c) const{
    return (this->r == c.r && this->g == c.g && this->b == c.b);
}

RGBColor RGBColor::powc(float p) const{
    return RGBColor(pow(this->r, p), pow(this->g, p), pow(this->b, p));
}

float RGBColor::average() const{
    return ((this->r + this->g + this->b)/3);
}

RGBColor operator*(const float a, const RGBColor &c){
    return RGBColor(a*c.r, a*c.g, a*c.b);
}
