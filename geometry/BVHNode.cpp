#include "BVHNode.hpp"
#include "Geometry.hpp"
#include "../world/World.hpp"   // needed so ShadeInfo(world) can be constructed

#include <algorithm>   // std::sort, std::min, std::max
#include <cfloat>      // FLT_MAX

// ---------------------------------------------------------------------------
// Global performance counters
// ---------------------------------------------------------------------------
long long g_bvh_nodes_visited = 0;
long long g_bvh_leaves_tested = 0;
long long g_bvh_prunes        = 0;

void reset_bvh_counters() {
    g_bvh_nodes_visited = 0;
    g_bvh_leaves_tested = 0;
    g_bvh_prunes        = 0;
}

// ---------------------------------------------------------------------------
// merge_bbox
//

BBox merge_bbox(const BBox& a, const BBox& b) {
    Point3D pmin(
        std::min(a.pmin.x, b.pmin.x),
        std::min(a.pmin.y, b.pmin.y),
        std::min(a.pmin.z, b.pmin.z)
    );
    Point3D pmax(
        std::max(a.pmax.x, b.pmax.x),
        std::max(a.pmax.y, b.pmax.y),
        std::max(a.pmax.z, b.pmax.z)
    );
    return BBox(pmin, pmax);
}

// ---------------------------------------------------------------------------
// Helpers for remove_region
// ---------------------------------------------------------------------------


static bool bbox_inside(const BBox& a, const BBox& b) {
    return (a.pmin.x >= b.pmin.x && a.pmax.x <= b.pmax.x &&
            a.pmin.y >= b.pmin.y && a.pmax.y <= b.pmax.y &&
            a.pmin.z >= b.pmin.z && a.pmax.z <= b.pmax.z);
}


static bool bbox_overlap(const BBox& a, const BBox& b) {
    if (a.pmax.x < b.pmin.x || a.pmin.x > b.pmax.x) return false;
    if (a.pmax.y < b.pmin.y || a.pmin.y > b.pmax.y) return false;
    if (a.pmax.z < b.pmin.z || a.pmin.z > b.pmax.z) return false;
    return true;
}

// ---------------------------------------------------------------------------
// BVHNode constructor / destructor
// ---------------------------------------------------------------------------
BVHNode::BVHNode()
    : left(nullptr), right(nullptr), object(nullptr) {}

BVHNode::~BVHNode() {
    delete left;
    delete right;
}

bool BVHNode::is_leaf() const {
    return object != nullptr;
}

// ---------------------------------------------------------------------------
// BVHNode::build - recursive longest-axis median split
//
// 
// how this is working ->>>>
// 1. base case: 1 object -> create a leaf.
// 2. compute centroid of each object's AABB.
// 3. recurse on each half.
// 4. internal node's AABB = merge of children's AABBs.
// ---------------------------------------------------------------------------
BVHNode* BVHNode::build(std::vector<Geometry*>& objects, int start, int end) {
    BVHNode* node = new BVHNode();
    int count = end - start;

    if (count == 1) {
        node->object = objects[start];
        node->bbox   = objects[start]->getBBox();
        return node;
    }

    float cx_min =  FLT_MAX, cy_min =  FLT_MAX, cz_min =  FLT_MAX;
    float cx_max = -FLT_MAX, cy_max = -FLT_MAX, cz_max = -FLT_MAX;

    for (int i = start; i < end; i++) {
        BBox b = objects[i]->getBBox();
        // 2 * centroid; the factor of 2 cancels in comparisons.
        float cx = b.pmin.x + b.pmax.x;
        float cy = b.pmin.y + b.pmax.y;
        float cz = b.pmin.z + b.pmax.z;
        cx_min = std::min(cx_min, cx);  cx_max = std::max(cx_max, cx);
        cy_min = std::min(cy_min, cy);  cy_max = std::max(cy_max, cy);
        cz_min = std::min(cz_min, cz);  cz_max = std::max(cz_max, cz);
    }

    float dx = cx_max - cx_min;
    float dy = cy_max - cy_min;
    float dz = cz_max - cz_min;

    int axis = 0;
    if (dy > dx && dy > dz) axis = 1;
    if (dz > dx && dz > dy) axis = 2;

    std::sort(objects.begin() + start, objects.begin() + end,
        [axis](Geometry* a, Geometry* b) {
            BBox ba = a->getBBox();
            BBox bb = b->getBBox();
            float ca = (axis == 0) ? (ba.pmin.x + ba.pmax.x)
                     : (axis == 1) ? (ba.pmin.y + ba.pmax.y)
                                   : (ba.pmin.z + ba.pmax.z);
            float cb = (axis == 0) ? (bb.pmin.x + bb.pmax.x)
                     : (axis == 1) ? (bb.pmin.y + bb.pmax.y)
                                   : (bb.pmin.z + bb.pmax.z);
            return ca < cb;
        });

    int mid = start + count / 2;
    node->left  = build(objects, start, mid);
    node->right = build(objects, mid,   end);
    node->bbox  = merge_bbox(node->left->bbox, node->right->bbox);
    return node;
}

void BVHNode::hit(const Ray& ray, float& tmin, ShadeInfo& result,
                  const World& world) const
{
    ++g_bvh_nodes_visited;

    // ---- AABB test -----
    float t_enter, t_exit;
    if (!bbox.hit(ray, t_enter, t_exit)) {
        ++g_bvh_prunes;
        return;
    }
    if (t_enter > tmin) {
        ++g_bvh_prunes;
        return;
    }

    // ---- Leaf test ----
    if (is_leaf()) {
        ++g_bvh_leaves_tested;
        ShadeInfo sr2(world);
        float t;
        if (object->hit(ray, t, sr2) && t < tmin) {
            tmin   = t;
            result = sr2;
        }
        return;
    }

    // ---- Internal node: visit NEAR child first ----
    // Order children by their AABB entry distance so tmin tightens early.
    float tL_enter, tL_exit, tR_enter, tR_exit;
    bool hitL = left ->bbox.hit(ray, tL_enter, tL_exit);
    bool hitR = right->bbox.hit(ray, tR_enter, tR_exit);

    if (hitL && hitR) {
        if (tL_enter <= tR_enter) {
            left ->hit(ray, tmin, result, world);
            right->hit(ray, tmin, result, world);
        } else {
            right->hit(ray, tmin, result, world);
            left ->hit(ray, tmin, result, world);
        }
    } else if (hitL) {
        left ->hit(ray, tmin, result, world);
    } else if (hitR) {
        right->hit(ray, tmin, result, world);
    }
}


bool BVHNode::in_shadow(const Ray& ray, float light_distance,
                        const World& world) const
{
    ++g_bvh_nodes_visited;

    float t_enter, t_exit;
    if (!bbox.hit(ray, t_enter, t_exit)) { ++g_bvh_prunes; return false; }
    if (t_enter > light_distance)        { ++g_bvh_prunes; return false; }

    if (is_leaf()) {
        ++g_bvh_leaves_tested;
        float t;
        ShadeInfo dummy(world);
        return object->hit(ray, t, dummy) && t < light_distance;
    }

    // Short-circuit: as soon as either side reports shadow, stop.
    return left ->in_shadow(ray, light_distance, world)
        || right->in_shadow(ray, light_distance, world);
}

// ---------------------------------------------------------------------------
// collect_leaves - flatten subtree to a list of Geometry pointers
// ---------------------------------------------------------------------------
void BVHNode::collect_leaves(std::vector<Geometry*>& out) const {
    if (is_leaf()) {
        out.push_back(object);
        return;
    }
    if (left)  left ->collect_leaves(out);
    if (right) right->collect_leaves(out);
}


int BVHNode::count_in_region(const BBox& region) const {
    if (!bbox_overlap(bbox, region)) return 0;

    if (bbox_inside(bbox, region)) {
        // Whole subtree matches - count leaves via flatten.
        std::vector<Geometry*> tmp;
        collect_leaves(tmp);
        return (int)tmp.size();
    }

    if (is_leaf()) {
        // Partial overlap on a leaf: include it only if the LEAF's bbox
        BBox lb = object->getBBox();
        return bbox_inside(lb, region) ? 1 : 0;
    }

    return (left  ? left ->count_in_region(region) : 0)
         + (right ? right->count_in_region(region) : 0);
}

static void collect_survivors_and_removed(
        BVHNode* node, const BBox& region,
        std::vector<Geometry*>& survivors,
        std::vector<Geometry*>& removed)
{
    if (!node) return;

    // PRUNE: subtree's bbox doesn't overlap region - keep entire subtree.
    if (!bbox_overlap(node->bbox, region)) {
        node->collect_leaves(survivors);
        return;
    }

    // FAST-DELETE: subtree fully inside region - delete entire subtree.
    if (bbox_inside(node->bbox, region)) {
        node->collect_leaves(removed);
        return;
    }

    // Partial overlap: must descend.
    if (node->is_leaf()) {
        BBox lb = node->object->getBBox();
        if (bbox_inside(lb, region)) removed .push_back(node->object);
        else                          survivors.push_back(node->object);
        return;
    }
    collect_survivors_and_removed(node->left,  region, survivors, removed);
    collect_survivors_and_removed(node->right, region, survivors, removed);
}

BVHNode* BVHNode::remove_region(BVHNode* root,
                                const BBox& region,
                                std::vector<Geometry*>& removed_out)
{
    if (!root) return nullptr;

    std::vector<Geometry*> survivors;
    collect_survivors_and_removed(root, region, survivors, removed_out);

    // Free the old tree (does not delete geometry pointers).
    delete root;

    if (survivors.empty()) return nullptr;
    return BVHNode::build(survivors, 0, (int)survivors.size());
}
