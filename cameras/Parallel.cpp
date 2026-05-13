#include "Parallel.hpp"

Parallel::Parallel()
{
    this->dir = Vector3D(0.0, 0.0, -1);  //Direction set parallel to negative z axis
}


Parallel::Parallel(float c)
{
    this->dir = Vector3D(c, c, c);  //Direction parallel to the written vector
}


Parallel::Parallel(float x, float y, float z)
{
    this->dir = Vector3D(x, y, z);  //Direction parallel to the vector (x,y,z)
}


Parallel::Parallel(const Vector3D &d)
{
    this->dir = d;  //Direction parallel to d
}


Parallel::Parallel(const Parallel &camera)
{
    this->dir = camera.dir;  //Copy constructor that sets direction parallel to direction of the variable camera
}


Parallel& Parallel::operator=(const Parallel &other)
{
    //Operator Overload to assign Direction
    this->dir = other.dir;
    return *this;
}


Vector3D Parallel::get_direction(const Point3D &p) const
{
    return dir;  //In parallel mode all vertices have the same dir vector.
}
