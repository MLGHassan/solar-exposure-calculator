#pragma once


#include <vector>
#include <string>

#include "../utilities/RGBColor.hpp"
#include "ViewPlane.hpp"
#include "../geometry/Geometry.hpp"
#include "../samplers/Sampler.hpp"
#include "../lights/Light.hpp"

class Camera;
class Geometry;
class Ray;
class Sampler;
class ShadeInfo;
class Normal;
class BVHNode;
class PointLight;

class World {
public:
  ViewPlane vplane;
  RGBColor bg_color;
  std::vector<Geometry *> geometry;
  std::vector<Geometry *> unbounded_geometry;
  std::vector<Light*> lights;
  Camera *camera_ptr;
  Sampler *sampler_ptr;
  BVHNode *bvh_root;


  PointLight* movable_light;

public:
  World();
  ~World();

  void add_geometry(Geometry *geom_ptr);
  void set_camera(Camera *c_ptr);


  void build();


  void clear_scene();


  void build_common_setup();

  void rebuild_bvh();

  bool build_from_config(const std::string& path);


  void set_light_position(float x, float y, float z);

  ShadeInfo hit_objects(const Ray &ray);
  bool in_shadow(const Ray& shadow_ray, float light_distance) const;

  int delete_in_region(const BBox& region);

  float compute_exposure(int samples, float area_half, float floor_y);
};
