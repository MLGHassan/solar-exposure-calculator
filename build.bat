@echo off
REM Build the raytracer on Windows (MinGW / MSYS2 g++).
g++ -std=c++17 -O2 -w ^
    raytracer.cpp ^
    world\World.cpp world\ViewPlane.cpp ^
    geometry\Geometry.cpp geometry\Sphere.cpp geometry\Plane.cpp ^
    geometry\Triangle.cpp geometry\BVHNode.cpp ^
    cameras\Perspective.cpp cameras\Parallel.cpp ^
    materials\Matte.cpp materials\Cosine.cpp ^
    samplers\Sampler.cpp samplers\Simple.cpp ^
    lights\PointLight.cpp lights\DirectionalLight.cpp ^
    utilities\BBox.cpp utilities\Image.cpp utilities\Point3D.cpp ^
    utilities\Ray.cpp utilities\RGBColor.cpp utilities\ShadeInfo.cpp ^
    utilities\Vector3D.cpp ^
    -o raytracer.exe
echo Built raytracer.exe
echo Run the GUI with:  python gui.py
