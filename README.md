# BVH-Accelerated Raytracer with Shadow Analysis

A C++ raytracer where every operation — visibility, shadows, scene CRUD,
and the headline analysis feature (solar exposure) — is powered by a
**Bounding Volume Hierarchy (BVH)**. A small Tkinter GUI lets you compose
scenes and move the light interactively.

---

## Quick start

```bash
# 1. Build the C++ raytracer
bash build.sh                # Linux / macOS
build.bat                    # Windows (MinGW g++)

# 2. Launch the GUI
python3 gui.py
```

The GUI shows a 400×400 rendered scene, a row of checkboxes for which
shapes to include, arrow buttons that move the (single) movable point
light, and a stats panel that updates after every render.

Optional: `pip install pillow` makes image loading ~10× faster. The GUI
falls back to a pure-Tkinter loader if Pillow isn't available.

