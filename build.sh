#!/usr/bin/env bash
# Build the raytracer binary.
# Linux/macOS: bash build.sh
# Windows:     use build.bat or run the g++ command from MSYS2/MinGW.
set -e
g++ -std=c++17 -O2 -w \
    raytracer.cpp \
    world/World.cpp world/ViewPlane.cpp \
    geometry/Geometry.cpp geometry/Sphere.cpp geometry/Plane.cpp \
    geometry/Triangle.cpp geometry/BVHNode.cpp \
    cameras/Perspective.cpp cameras/Parallel.cpp \
    materials/Matte.cpp materials/Cosine.cpp \
    samplers/Sampler.cpp samplers/Simple.cpp \
    lights/PointLight.cpp lights/DirectionalLight.cpp \
    utilities/BBox.cpp utilities/Image.cpp utilities/Point3D.cpp \
    utilities/Ray.cpp utilities/RGBColor.cpp utilities/ShadeInfo.cpp \
    utilities/Vector3D.cpp \
    -o raytracer
echo "Built ./raytracer"
echo "Run the GUI with:  python3 gui.py"
