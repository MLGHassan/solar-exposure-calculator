#include "Geometry.hpp"

Geometry::Geometry()
{
    this->material_ptr = nullptr;
}

Material* Geometry::get_material() const{
    return this->material_ptr;
}

void Geometry::set_material(Material *mPtr)
{
    this->material_ptr = mPtr;
}
