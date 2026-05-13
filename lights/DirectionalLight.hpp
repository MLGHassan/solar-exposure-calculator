#pragma once
#include "Light.hpp"

// ---------------------------------------------------------------------------
// DirectionalLight — a light infinitely far away (like the sun).
//
// The direction toward the light is constant across the entire scene.
// Because it is at infinite distance, shadow rays have no length limit.
// ---------------------------------------------------------------------------
class DirectionalLight : public Light {
public:
    DirectionalLight();
    DirectionalLight(const Vector3D& dir, const RGBColor& col, float intensity);

    virtual Vector3D get_direction(const ShadeInfo& sr) const override;
    virtual RGBColor L(const ShadeInfo& sr)             const override;
    virtual float    distance(const ShadeInfo& sr)      const override;

    void set_direction(const Vector3D& d) { direction = d; direction.normalize(); }
    void set_color(const RGBColor& c)     { color = c; }
    void set_intensity(float i)           { intensity = i; }

private:
    Vector3D direction;  // direction FROM the light TOWARD the scene (normalized)
};
