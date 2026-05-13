#include "Image.hpp"
#include <fstream>
#include <string>
#include <algorithm> // For std::max if you ever need it

Image::Image(int hres, int vres){
    // Assign hres and vres from parameters
    this->hres = hres;
    this->vres = vres;

    // Assign color pointer array (rows / height)
    this->colors = new RGBColor*[this->vres];
    // Assign color array (columns / width)
    for (int i=0; i<(this->vres); i++)
        colors[i] = new RGBColor[this->hres];
}

Image::Image(const ViewPlane &vp){
    // Assign hres and vres from Viewplane
    this->hres = vp.get_hres();
    this->vres = vp.get_vres();

    // Assign color pointer array
    this->colors = new RGBColor*[this->vres];
    // Assign color array
    for (int i=0; i<(this->vres); i++)
        colors[i] = new RGBColor[this->hres];
}

Image::~Image(){
    for (int i = 0; i < this->vres; i++){
        // Delete pointers of each RGBcolor array
        delete [] this->colors[i];
    }
    // Delete color**
    delete [] this->colors;
}

void Image::set_pixel(int x, int y, const RGBColor& color){
    // FIX 1: The array was allocated as [vres][hres] (height then width).
    // Therefore, it MUST be accessed as [y][x], not [x][y]!
    this->colors[y][x] = color;
}

void Image::write_ppm(std::string path) const{
    std::ofstream file(path);
    
    // Write the strict PPM header
    file << "P3\n";
    file << hres << " " << vres << "\n";
    file << "255\n"; // 255 is the absolute maximum integer value for colors

    // Loop through rows (y) then columns (x)
    for (int y = 0; y < this->vres; y++){
        for(int x = 0; x < this->hres; x++){
            
            // 1. Get the color from the properly indexed array
            RGBColor c = this->colors[y][x];

            // FIX 2: Multiply by 255 and cast to an INTEGER. 
            // The PPM format breaks if you use std::to_string() on a float because it writes decimals.
            int r = static_cast<int>(c.r * 255.0f);
            int g = static_cast<int>(c.g * 255.0f);
            int b = static_cast<int>(c.b * 255.0f);

            // FIX 3: Clamp the values between 0 and 255.
            // This prevents the math from "overflowing" and causing that TV static noise.
            if (r > 255) r = 255; 
            if (r < 0) r = 0;
            
            if (g > 255) g = 255; 
            if (g < 0) g = 0;
            
            if (b > 255) b = 255; 
            if (b < 0) b = 0;

            // 4. Write pure, space-separated integers to the file
            file << r << " " << g << " " << b << " ";
        }
        file << "\n"; // New line for each row makes the file readable
    }
    file.close();
}