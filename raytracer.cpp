#include <iostream>
#include <string>
#include <cstring>
#include <vector>

#include "samplers/Sampler.hpp"
#include "utilities/Image.hpp"
#include "utilities/RGBColor.hpp"
#include "utilities/Ray.hpp"
#include "utilities/ShadeInfo.hpp"
#include "utilities/BBox.hpp"
#include "utilities/Point3D.hpp"

#include "world/World.hpp"
#include "world/ViewPlane.hpp"
#include "geometry/BVHNode.hpp"
#include "materials/Material.hpp"


int main(int argc, char** argv) {
    std::string config_path = "";
    std::string out_path    = "scene.ppm";
    int  exposure_samples   = 0;
    bool do_delete          = false;
    float dx0=0, dy0=0, dz0=0, dx1=0, dy1=0, dz1=0;

    for (int i = 1; i < argc; ++i) {
        std::string a = argv[i];
        if (a == "--scene" && i + 1 < argc)            config_path = argv[++i];
        else if (a == "--out" && i + 1 < argc)         out_path    = argv[++i];
        else if (a == "--exposure-samples" && i+1 < argc)
            exposure_samples = std::stoi(argv[++i]);
        else if (a == "--delete-region" && i + 6 < argc) {
            do_delete = true;
            dx0 = std::stof(argv[++i]); dy0 = std::stof(argv[++i]); dz0 = std::stof(argv[++i]);
            dx1 = std::stof(argv[++i]); dy1 = std::stof(argv[++i]); dz1 = std::stof(argv[++i]);
        }
        else {
            std::cerr << "[raytracer] ignoring unknown arg: " << a << "\n";
        }
    }

    World world;
    if (!config_path.empty()) {
        if (!world.build_from_config(config_path)) {
            std::cerr << "[raytracer] failed to load config; falling back to default scene\n";
            world.clear_scene();
            world.build();
        }
    } else {
        world.build();
    }

    // --------------- Feature 5: bulk delete via BVH spatial query ----------
    int removed_count = 0;
    if (do_delete) {
        // Order the corners min/max in case the caller passed them swapped.
        Point3D pmin(std::min(dx0,dx1), std::min(dy0,dy1), std::min(dz0,dz1));
        Point3D pmax(std::max(dx0,dx1), std::max(dy0,dy1), std::max(dz0,dz1));
        BBox region(pmin, pmax);
        removed_count = world.delete_in_region(region);
    }

    // --------------- Render --------------------------------------------
    reset_bvh_counters();

    Sampler*   sampler   = world.sampler_ptr;
    ViewPlane& viewplane = world.vplane;
    Image image(viewplane);

    std::vector<Ray> rays;
    for (int x = 0; x < viewplane.hres; ++x) {
        for (int y = 0; y < viewplane.vres; ++y) {
            RGBColor pixel_color(0);
            rays = sampler->get_rays(x, y);
            for (const auto& ray : rays) {
                float weight = ray.w;
                ShadeInfo sinfo = world.hit_objects(ray);
                if (sinfo.hit)
                    pixel_color += weight * sinfo.material_ptr->shade(sinfo);
                else
                    pixel_color += weight * world.bg_color;
            }
            image.set_pixel(x, y, pixel_color);
        }
    }
    image.write_ppm(out_path);

    // --------------- Machine-readable summary for the GUI --------------
    std::cout << "WROTE " << out_path << "\n";
    std::cout << "BVH_NODES_VISITED " << g_bvh_nodes_visited << "\n";
    std::cout << "BVH_LEAVES_TESTED " << g_bvh_leaves_tested << "\n";
    std::cout << "BVH_PRUNES "        << g_bvh_prunes        << "\n";
    if (do_delete) std::cout << "REMOVED " << removed_count << "\n";

    // --------------- Feature 6: exposure --------------------------------
    if (exposure_samples > 0) {
        float exp = world.compute_exposure(exposure_samples,
                                           /*area_half=*/10.f,
                                           /*floor_y  =*/-5.f);
        std::cout << "EXPOSURE "                << exp                  << "\n";
        std::cout << "EXPOSURE_BVH_NODES "      << g_bvh_nodes_visited  << "\n";
        std::cout << "EXPOSURE_BVH_LEAVES "     << g_bvh_leaves_tested  << "\n";
        std::cout << "EXPOSURE_BVH_PRUNES "     << g_bvh_prunes         << "\n";
    }
    return 0;
}
