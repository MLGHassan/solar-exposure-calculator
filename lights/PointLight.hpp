#pragma once
#include "Light.hpp"

// ---------------------------------------------------------------------------
// PointLight — an infinitely small light at a fixed position in world space.
//
// The direction toward the light is simply (position - hit_point), normalized.
// Radiance is constant regardless of distance (no attenuation by default —
// easy to add later).
// ---------------------------------------------------------------------------
class PointLight : public Light {
public:
    PointLight();
    PointLight(const Point3D& pos, const RGBColor& col, float intensity);

    virtual Vector3D get_direction(const ShadeInfo& sr) const override;
    virtual RGBColor L(const ShadeInfo& sr)             const override;
    virtual float    distance(const ShadeInfo& sr)      const override;

    void set_position(const Point3D& p)  { position = p; }
    void set_color(const RGBColor& c)    { color = c; }
    void set_intensity(float i)          { intensity = i; }

private:
    Point3D position;
};
