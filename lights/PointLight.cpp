#include "PointLight.hpp"
#include "../utilities/ShadeInfo.hpp"
#include <cmath>

PointLight::PointLight()
    : position(0, 10, 0)
{
    color     = RGBColor(1, 1, 1);
    intensity = 1.0f;
}

PointLight::PointLight(const Point3D& pos, const RGBColor& col, float intens)
    : position(pos)
{
    color     = col;
    intensity = intens;
}

// Direction FROM the hit point TOWARD the light (normalized).
// This is the "l" vector in the diffuse shading equation: max(n · l, 0).
Vector3D PointLight::get_direction(const ShadeInfo& sr) const {
    Vector3D dir = position - sr.hit_point;
    dir.normalize();
    return dir;
}

// Radiance = color * intensity (no distance attenuation for simplicity).
RGBColor PointLight::L(const ShadeInfo& sr) const {
    return color * intensity;
}

// Euclidean distance from hit point to the light position.
// Used in World::in_shadow to avoid testing geometry beyond the light.
float PointLight::distance(const ShadeInfo& sr) const {
    return (float)(position - sr.hit_point).length();
}
