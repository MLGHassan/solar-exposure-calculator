#include "ShadeInfo.hpp"

ShadeInfo::ShadeInfo(const World &wr){

    this->hit = false;
    this->t = kHugeValue;
    this->w = &wr;

}