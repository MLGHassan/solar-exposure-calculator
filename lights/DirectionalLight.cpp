#include "DirectionalLight.hpp"
#include "../utilities/ShadeInfo.hpp"
#include "../utilities/Constants.hpp"

DirectionalLight::DirectionalLight()
{
    direction = Vector3D(0, -1, 0);   // pointing straight down
    direction.normalize();
    color     = RGBColor(1, 1, 1);
    intensity = 1.0f;
}

DirectionalLight::DirectionalLight(const Vector3D& dir, const RGBColor& col, float intens)
    : direction(dir)
{
    direction.normalize();
    color     = col;
    intensity = intens;
}

// The direction toward the light is the NEGATION of the light's direction vector.
// (direction stores "light → scene"; we need "surface → light")
Vector3D DirectionalLight::get_direction(const ShadeInfo& sr) const {
    return -direction;
}

RGBColor DirectionalLight::L(const ShadeInfo& sr) const {
    return color * intensity;
}

// Directional lights are at infinity — return a huge value so shadow rays
// are never clipped short when comparing against the light distance.
float DirectionalLight::distance(const ShadeInfo& sr) const {
    return kHugeValue;
}
