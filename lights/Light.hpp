#pragma once
#include "../utilities/Vector3D.hpp"
#include "../utilities/Point3D.hpp"
#include "../utilities/RGBColor.hpp"
#include "../utilities/Ray.hpp"

// Forward declare so we don't pull in all of ShadeInfo here
struct ShadeInfo;

// ---------------------------------------------------------------------------
// Light — abstract base class
//
// Every light must answer two questions:
//   get_direction(sr) — which direction does light arrive FROM at the hit point?
//                       (i.e. the vector pointing FROM surface TOWARD the light)
//   L(sr)             — what is the radiance (color * intensity) of this light?
// ---------------------------------------------------------------------------
class Light {
public:
    Light() : color(1,1,1), intensity(1.0f), casts_shadows(true) {}
    virtual ~Light() = default;

    // Direction FROM the hit point TOWARD the light source (unit vector)
    virtual Vector3D get_direction(const ShadeInfo& sr) const = 0;

    // Radiance emitted by this light
    virtual RGBColor L(const ShadeInfo& sr) const = 0;

    // Distance from hit point to the light (used to clamp shadow ray length).
    // Returns a very large number for directional lights (infinite distance).
    virtual float distance(const ShadeInfo& sr) const = 0;

    bool casts_shadows;

protected:
    RGBColor color;
    float    intensity;
};
