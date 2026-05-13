#pragma once
#include "../materials/Material.hpp"
#include "../utilities/RGBColor.hpp"

// ---------------------------------------------------------------------------
// Matte — ambient + Lambertian (diffuse) shading.
//
// Shade equation per light:
//   color += kd * diffuse_color * (n · l) * L_light
//
// Where:
//   kd            = diffuse coefficient  [0,1]
//   diffuse_color = surface color
//   n             = shading normal (unit)
//   l             = direction toward light (unit)
//   L_light       = light radiance (color * intensity)
//
// If the point is in shadow (World::in_shadow returns true) the diffuse
// contribution from that light is skipped; ambient is always added.
// ---------------------------------------------------------------------------
class Matte : public Material {
public:
    Matte();
    Matte(const RGBColor& color);
    Matte(float ka, float kd, const RGBColor& color);

    virtual RGBColor shade(const ShadeInfo& sr) const override;

    void set_ka(float a)             { ka = a; }
    void set_kd(float d)             { kd = d; }
    void set_color(const RGBColor& c){ diffuse_color = c; }

private:
    float    ka;             // ambient coefficient
    float    kd;             // diffuse coefficient
    RGBColor diffuse_color;
};
