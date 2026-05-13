#include "Triangle.hpp"

Triangle::Triangle()
{
    v0 = Point3D();
    v1 = Point3D();
    v2 = Point3D();
}

Triangle::Triangle(const Point3D &v0_, const Point3D &v1_, const Point3D &v2_)
{
    v0 = v0_;//operator overloading
    v1 = v1_;
    v2 = v2_;
}

Triangle::Triangle(const Triangle &object)
{
    v0 = object.v0;
    v1 = object.v1;
    v2 = object.v2;
}

Triangle &Triangle::operator=(const Triangle &rhs)
{
    v0 = rhs.v0;
    v1 = rhs.v1;
    v2 = rhs.v2;
    return *this;
}

std::string Triangle::to_string() const
{
     return "Vertex 0:" + v0.to_string() + "\n" + "Vertex 1:" + v1.to_string() +
           "\n" + "Vertex 2:" + v2.to_string() + "\n" ;
}

// implemented from book
bool Triangle::hit(const Ray &ray, float &tmin, ShadeInfo &sr) const
{
    float a = v0.x - v1.x, b = v0.x - v2.x, c = ray.d.x, d = v0.x - ray.o.x;
    float e = v0.y - v1.y, f = v0.y - v2.y, g = ray.d.y, h = v0.y - ray.o.y;
    float i = v0.z - v1.z, j = v0.z - v2.z, k = ray.d.z, l = v0.z - ray.o.z;
    Vector3D v_0 = this->v1 - this->v0;
    Vector3D v_1 = this->v2 - this->v0;
    Vector3D v_2 = v_0^v_1;
    float m = f * k - g * j, n = h * k - g * l, p = f * l - h * j;
    float q = g * i - e * k, s = e * j - f * i;

    float inv_denom = 1.0f / (a * m + b * q + c * s);

    float e1 = d * m - b * n - c * p;
    float beta = e1 * inv_denom;

    if (beta < 0.0f)
    {
        return false;
    }

    float r = r = e * l - h * i;
    float e2 = a * n + d * q + c * r;
    float gamma = e2 * inv_denom;

    if (gamma < 0.0f)
    {
        return false;
    }

    if (beta + gamma > 1.0f)
    {
        return false;
    }

    float e3 = a * p - b * r + d * s;
    float t = e3 * inv_denom;

    if (t < kEpsilon)
    {
        return false;
    }

    tmin = t;
    sr.hit_point = ray.o + t * ray.d;
	sr.ray = ray;
    sr.t = t;
    sr.hit = true;
    sr.normal = v_2;
    sr.normal.normalize();
    // Two-sided shading: flip the normal so it always faces the incoming ray.
    // Required because the cube/square primitives are built from triangles
    // whose winding may not match the light direction, and we want all faces
    // (front and back) to shade correctly.
    if (sr.normal * ray.d > 0.0) sr.normal = -sr.normal;
    sr.material_ptr = this->material_ptr;
    return true;
}

BBox Triangle::getBBox() const
{   Point3D start = min(v0, min(v1, v2));
    Point3D end = max(v0, max(v1, v2));
    // Pad degenerate axes. Axis-aligned triangles (cube faces, flat quads)
    // have zero extent on one axis, which makes the BBox-vs-ray slab test
    // unstable for rays that don't have a nonzero component on that axis.
    // A tiny pad makes the AABB unambiguously 3D without affecting the
    // actual triangle's hit test.
    const float pad = 1e-4f;
    if (end.x - start.x < pad) { start.x -= pad; end.x += pad; }
    if (end.y - start.y < pad) { start.y -= pad; end.y += pad; }
    if (end.z - start.z < pad) { start.z -= pad; end.z += pad; }
    return BBox(start, end);
}
