#pragma once

/**
    general structure of the BVHNode

   Each node stores an AABB (BBox) that encloses all geometry in its subtree.
   Leaf nodes store a single Geometry* object.
   Internal nodes store left/right child pointers and a merged AABB.

   Build:    BVHNode::build(objects, 0, objects.size())
   Traverse: node->hit(ray, tmin, result, world)
**/

#include <vector>
#include "../utilities/BBox.hpp"
#include "../utilities/Ray.hpp"
#include "../utilities/ShadeInfo.hpp"

class Geometry;
class World;

extern long long g_bvh_nodes_visited;
extern long long g_bvh_leaves_tested;
extern long long g_bvh_prunes;

void reset_bvh_counters();

// Merges two bounding boxes into one that tightly contains both.
// Used bottom-up during tree construction to compute parent AABBs.
BBox merge_bbox(const BBox& a, const BBox& b);

class BVHNode {
public:
    BBox     bbox;    // AABB enclosing this entire subtree
    BVHNode* left;    // left child  (nullptr if this is a leaf)
    BVHNode* right;   // right child (nullptr if this is a leaf)
    Geometry* object; // non-null ONLY for leaf nodes

    BVHNode();
    ~BVHNode();

    bool is_leaf() const;

    static BVHNode* build(std::vector<Geometry*>& objects, int start, int end);


    void hit(const Ray& ray, float& tmin, ShadeInfo& result, const World& world) const;

    bool in_shadow(const Ray& ray, float light_distance, const World& world) const;

    static BVHNode* remove_region(BVHNode* root,
                                  const BBox& region,
                                  std::vector<Geometry*>& removed_out);

    int count_in_region(const BBox& region) const;


    void collect_leaves(std::vector<Geometry*>& out) const;
};
