#include "World.hpp"
#include "../geometry/BVHNode.hpp"

#include "../cameras/Perspective.hpp"
#include "../cameras/Parallel.hpp"

#include "../geometry/Plane.hpp"
#include "../geometry/Sphere.hpp"
#include "../geometry/Triangle.hpp"

#include "../materials/Cosine.hpp"
#include "../materials/Matte.hpp"

#include "../samplers/Simple.hpp"

#include "../utilities/Constants.hpp"

#include "../lights/PointLight.hpp"
#include "../lights/DirectionalLight.hpp"

#include <fstream>
#include <sstream>
#include <iostream>
#include <algorithm>
#include <cmath>

// ---------------------------------------------------------------------------
// ctor/dtor
// ---------------------------------------------------------------------------
World::World()
{
    vplane         = ViewPlane();
    bg_color       = RGBColor();
    camera_ptr     = nullptr;
    sampler_ptr    = nullptr;
    bvh_root       = nullptr;
    movable_light  = nullptr;
}

World::~World() {
    clear_scene();
    delete camera_ptr;
    delete sampler_ptr;
}

void World::add_geometry(Geometry *geom_ptr) {
    geometry.push_back(geom_ptr);
}

void World::set_camera(Camera *c_ptr) {
    camera_ptr = c_ptr;
}

// ---------------------------------------------------------------------------
// clear_scene - free geometry, lights, BVH. Leaves camera/sampler/viewplane

void World::clear_scene() {
    delete bvh_root;
    bvh_root = nullptr;

    // Only `geometry` owns the objects - `unbounded_geometry` aliases into
    // it (set up by rebuild_bvh). Free once, clear both.
    for (auto g : geometry) delete g;
    geometry.clear();
    unbounded_geometry.clear();

    for (auto l : lights) delete l;
    lights.clear();
    movable_light = nullptr;
}

// ---------------------------------------------------------------------------
// rebuild_bvh - partition geometry by boundedness, then build the tree.
//

void World::rebuild_bvh() {
    delete bvh_root;
    bvh_root = nullptr;
    unbounded_geometry.clear();

    std::vector<Geometry*> bounded;
    for (auto g : geometry) {
        BBox b = g->getBBox();
        bool degenerate = (b.pmin.x == b.pmax.x &&
                           b.pmin.y == b.pmax.y &&
                           b.pmin.z == b.pmax.z);
        if (degenerate) unbounded_geometry.push_back(g);
        else            bounded.push_back(g);
    }
    if (!bounded.empty())
        bvh_root = BVHNode::build(bounded, 0, (int)bounded.size());
}

// ---------------------------------------------------------------------------
// build_common_setup - camera, sampler, viewplane, floor plane.
//
// The floor is non user-removable 
// ---------------------------------------------------------------------------
void World::build_common_setup() {
    int n = 10;
    vplane.top_left.x     = -n;
    vplane.top_left.y     =  n;
    vplane.top_left.z     =  10;
    vplane.bottom_right.x =  n;
    vplane.bottom_right.y = -n;
    vplane.bottom_right.z =  10;
    vplane.hres = 400;
    vplane.vres = 400;

    bg_color = RGBColor(0.08f, 0.08f, 0.10f);

    delete camera_ptr;
    camera_ptr = new Perspective(0, 5, 20);

    delete sampler_ptr;
    sampler_ptr = new Simple(camera_ptr, &vplane);

    // Floor (unbounded plane, sits at y = -5)
    Plane* floor = new Plane(Point3D(0, -5, 0), Vector3D(0, 1, 0));
    floor->set_material(new Matte(0.15f, 0.85f, RGBColor(0.6f, 0.6f, 0.6f)));
    add_geometry(floor);


    DirectionalLight* dl = new DirectionalLight(
        Vector3D(1, -0.5f, 0.5f),
        RGBColor(0.4f, 0.55f, 0.9f),
        0.3f
    );
    dl->casts_shadows = false;
    lights.push_back(dl);

    // movable point light
    movable_light = new PointLight(
        Point3D(0, 15, 5),
        RGBColor(1.0f, 0.95f, 0.8f),
        2.0f
    );
    lights.push_back(movable_light);
}


static RGBColor mk_color(float r, float g, float b) { return RGBColor(r, g, b); }

static void add_cube(World& w, float cx, float cy, float cz, float h,
                     const RGBColor& col)
{
    // 8 corners of an axis-aligned cube of half-size h.
    Point3D p[8] = {
        Point3D(cx-h, cy-h, cz-h), Point3D(cx+h, cy-h, cz-h),
        Point3D(cx+h, cy+h, cz-h), Point3D(cx-h, cy+h, cz-h),
        Point3D(cx-h, cy-h, cz+h), Point3D(cx+h, cy-h, cz+h),
        Point3D(cx+h, cy+h, cz+h), Point3D(cx-h, cy+h, cz+h),
    };
    // 12 triangles - 2 per face. Vertex order chosen so the outward normal
    // points away from the cube center on each face.
    int faces[12][3] = {
        {0,1,2},{0,2,3},  // back  (-z)
        {5,4,7},{5,7,6},  // front (+z)
        {4,0,3},{4,3,7},  // left  (-x)
        {1,5,6},{1,6,2},  // right (+x)
        {3,2,6},{3,6,7},  // top   (+y)
        {4,5,1},{4,1,0},  // bot   (-y)
    };
    for (int i = 0; i < 12; ++i) {
        Triangle* t = new Triangle(p[faces[i][0]], p[faces[i][1]], p[faces[i][2]]);
        t->set_material(new Matte(0.2f, 0.8f, col));
        w.add_geometry(t);
    }
}

static void add_square(World& w, float cx, float cy, float cz, float h,
                       const RGBColor& col)
{
    // Quad lying in the XY plane facing +Z. Two triangles. Triangle::getBBox
    Point3D p0(cx-h, cy-h, cz);
    Point3D p1(cx+h, cy-h, cz);
    Point3D p2(cx+h, cy+h, cz);
    Point3D p3(cx-h, cy+h, cz);

    Triangle* t1 = new Triangle(p0, p1, p2);
    Triangle* t2 = new Triangle(p0, p2, p3);
    t1->set_material(new Matte(0.2f, 0.8f, col));
    t2->set_material(new Matte(0.2f, 0.8f, col));
    w.add_geometry(t1);
    w.add_geometry(t2);
}

// ---------------------------------------------------------------------------
// Each line is independent. Unknown lines are ignored with a warning.
// ---------------------------------------------------------------------------
bool World::build_from_config(const std::string& path) {
    clear_scene();
    build_common_setup();

    std::ifstream f(path);
    if (!f) {
        std::cerr << "[World] could not open config: " << path << "\n";
        rebuild_bvh();
        return false;
    }

    std::string line;
    while (std::getline(f, line)) {
        if (line.empty() || line[0] == '#') continue;
        std::istringstream ss(line);
        std::string kind;
        ss >> kind;

        if (kind == "light") {
            float x, y, z; ss >> x >> y >> z;
            set_light_position(x, y, z);
        }
        else if (kind == "sphere") {
            float cx, cy, cz, r, R, G, B;
            ss >> cx >> cy >> cz >> r >> R >> G >> B;
            Sphere* s = new Sphere(Point3D(cx, cy, cz), r);
            s->set_material(new Matte(0.2f, 0.8f, mk_color(R, G, B)));
            add_geometry(s);
        }
        else if (kind == "triangle") {
            float x0,y0,z0, x1,y1,z1, x2,y2,z2, R,G,B;
            ss >> x0 >> y0 >> z0 >> x1 >> y1 >> z1 >> x2 >> y2 >> z2 >> R >> G >> B;
            Triangle* t = new Triangle(Point3D(x0,y0,z0), Point3D(x1,y1,z1),
                                       Point3D(x2,y2,z2));
            t->set_material(new Matte(0.2f, 0.8f, mk_color(R, G, B)));
            add_geometry(t);
        }
        else if (kind == "cube") {
            float cx, cy, cz, h, R, G, B;
            ss >> cx >> cy >> cz >> h >> R >> G >> B;
            add_cube(*this, cx, cy, cz, h, mk_color(R, G, B));
        }
        else if (kind == "square") {
            float cx, cy, cz, h, R, G, B;
            ss >> cx >> cy >> cz >> h >> R >> G >> B;
            add_square(*this, cx, cy, cz, h, mk_color(R, G, B));
        }
        else {
            std::cerr << "[World] unknown directive: " << kind << "\n";
        }
    }

    rebuild_bvh();
    return true;
}

void World::set_light_position(float x, float y, float z) {
    if (movable_light) movable_light->set_position(Point3D(x, y, z));
}

// ---------------------------------------------------------------------------
// hit_objects - BVH-accelerated nearest-intersection
// ---------------------------------------------------------------------------
ShadeInfo World::hit_objects(const Ray& ray) {
    ShadeInfo sr(*this);
    float tmin = kHugeValue;

    if (bvh_root) bvh_root->hit(ray, tmin, sr, *this);

    for (auto g : unbounded_geometry) {
        ShadeInfo sr2(*this);
        float t;
        if (g->hit(ray, t, sr2) && t < tmin) {
            tmin = t;
            sr   = sr2;
        }
    }
    return sr;
}

bool World::in_shadow(const Ray& shadow_ray, float light_distance) const {
    if (bvh_root && bvh_root->in_shadow(shadow_ray, light_distance, *this))
        return true;

    for (auto g : unbounded_geometry) {
        float t;
        ShadeInfo sr(*this);
        if (g->hit(shadow_ray, t, sr) && t < light_distance) return true;
    }
    return false;
}

// ---------------------------------------------------------------------------
//
// workflow to explain to miss!!! 
//   1. snapshot bounded geometry pointer set (for membership tests).
//   2. call BVHNode::remove_region which returns removed pointers + new root.
//   3. erase removed pointers from World::geometry and delete them.
//   4. rebuild unbounded list (unchanged in practice, but kept simple).
//   5. why is this so complicated
//
// ---------------------------------------------------------------------------
int World::delete_in_region(const BBox& region) {
    if (!bvh_root) return 0;

    std::vector<Geometry*> removed;
    bvh_root = BVHNode::remove_region(bvh_root, region, removed);

    // Erase removed pointers from the geometry vector and delete them.
    for (auto p : removed) {
        auto it = std::find(geometry.begin(), geometry.end(), p);
        if (it != geometry.end()) geometry.erase(it);
        delete p;
    }

    return (int)removed.size();
}

// ---------------------------------------------------------------------------

float World::compute_exposure(int samples, float area_half, float floor_y) {
    if (!movable_light) return 0.f;
    if (samples < 2) samples = 2;

    reset_bvh_counters();

    Point3D light_pos;
    {
        // we don't have a getter so we reconstruct the position of the light by probing 
        ShadeInfo sr(*this);
        sr.hit_point = Point3D(0, 0, 0);
        Vector3D dir = movable_light->get_direction(sr);  // toward light from origin
        float    dst = movable_light->distance(sr);
        light_pos = Point3D(0,0,0) + dir * dst;
    }

    Vector3D up_normal(0, 1, 0);

    int lit = 0;
    int total = samples * samples;
    float step = (2.0f * area_half) / (float)(samples - 1);

    for (int i = 0; i < samples; ++i) {
        for (int j = 0; j < samples; ++j) {
            float x = -area_half + i * step;
            float z = -area_half + j * step;
            Point3D sample(x, floor_y, z);

            Vector3D to_light = light_pos - sample;
            float    dist     = (float)to_light.length();
            Vector3D dir      = to_light;
            dir.normalize();

            // Self-shadow nudge: lift origin slightly along the floor normal.
            Ray shadow_ray(sample + up_normal * kEpsilon, dir);

            if (!in_shadow(shadow_ray, dist)) ++lit;
        }
    }

    return (float)lit / (float)total;
}


void World::build(void) {
    build_common_setup();

    Sphere* sphere1 = new Sphere(Point3D(-3, 0, 0), 3.5f);
    sphere1->set_material(new Matte(0.2f, 0.8f, RGBColor(0.8f, 0.15f, 0.1f)));
    add_geometry(sphere1);

    Sphere* sphere2 = new Sphere(Point3D(4, -2, -2), 2.5f);
    sphere2->set_material(new Matte(0.2f, 0.8f, RGBColor(0.1f, 0.7f, 0.2f)));
    add_geometry(sphere2);

    Triangle* tri = new Triangle(
        Point3D(-2, 5, -3), Point3D( 2, 5, -3), Point3D( 0, 9, 0));
    tri->set_material(new Matte(0.2f, 0.8f, RGBColor(0.9f, 0.8f, 0.2f)));
    add_geometry(tri);

    rebuild_bvh();
}
