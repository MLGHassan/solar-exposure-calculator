#include "Matte.hpp"
#include "../utilities/ShadeInfo.hpp"
#include "../world/World.hpp"
#include "../lights/Light.hpp"

Matte::Matte()
    : ka(0.25f), kd(0.75f), diffuse_color(1, 1, 1)
{}

Matte::Matte(const RGBColor& color)
    : ka(0.25f), kd(0.75f), diffuse_color(color)
{}

Matte::Matte(float ka_, float kd_, const RGBColor& color)
    : ka(ka_), kd(kd_), diffuse_color(color)
{}

// ---------------------------------------------------------------------------
// shade
//
// Steps:
//   1. Start with ambient: ka * diffuse_color
//   2. For each light in the world:
//      a. Get the direction toward the light: l = light.get_direction(sr)
//      b. Compute n·l (cosine of angle between normal and light direction)
//      c. If n·l > 0 the surface faces the light (not self-shadowed)
//      d. Cast a shadow ray from the hit point toward the light
//      e. If NOT in shadow: add  kd * diffuse_color * (n·l) * light.L(sr)
// ---------------------------------------------------------------------------
RGBColor Matte::shade(const ShadeInfo& sr) const {

    // Ambient contribution — always present, never shadowed
    RGBColor L = diffuse_color * ka;

    // Surface normal at the hit point (already normalised in hit())
    Vector3D normal = sr.normal;

    for (auto light : sr.w->lights) {

        // Direction from hit point toward this light
        Vector3D light_dir = light->get_direction(sr);

        // Lambertian cosine term
        float ndotl = (float)(normal * light_dir);

        // Surface faces the light — check for shadow before adding diffuse
        if (ndotl > 0.0f) {

            bool shadowed = false;

            if (light->casts_shadows) {
                // Shadow ray: origin = hit point, direction = toward light
                // The origin is nudged along the normal by kEpsilon to prevent
                // the surface from self-intersecting (shadow acne).
                Ray shadow_ray(sr.hit_point + normal * kEpsilon, light_dir);
                shadowed = sr.w->in_shadow(shadow_ray, light->distance(sr));
            }

            if (!shadowed) {
                // kd * color * (n·l) * light_radiance
                L += diffuse_color * kd * ndotl * light->L(sr);
            }
        }
    }

    return L;
}
