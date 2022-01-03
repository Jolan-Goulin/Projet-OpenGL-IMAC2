#include "glimac/Image.hpp"
#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"
#include <iostream>

namespace glimac {

std::unique_ptr<Image> loadImage(const FilePath& filepath) {
    int x, y, n;
    unsigned char *data = stbi_load(filepath.c_str(), &x, &y, &n, 4);
    if(!data) {
        std::cerr << "loading image " << filepath << " error: " << stbi_failure_reason() << std::endl;
        return std::unique_ptr<Image>();
    }
    std::unique_ptr<Image> pImage(new Image(x, y));
    unsigned int size = x * y;
    auto scale = 1.f / 255;
    auto ptr = pImage->getPixels();
    for(auto i = 0u; i < size; ++i) {
        auto offset = 4 * i;
        ptr->r = data[offset] * scale;
        ptr->g = data[offset + 1] * scale;
        ptr->b = data[offset + 2] * scale;
        ptr->a = data[offset + 3] * scale;
        ++ptr;
    }
    stbi_image_free(data);
    return pImage;
}

}