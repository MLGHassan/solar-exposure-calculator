#include "../samplers/Sampler.hpp"

Sampler::Sampler()
{
    this->camera_ptr = nullptr;
    this->viewplane_ptr = nullptr;
}

Sampler::Sampler(Camera *c_ptr, ViewPlane *v_ptr)
{
    this->camera_ptr = c_ptr;
    this->viewplane_ptr = v_ptr;

}

