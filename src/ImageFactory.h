#pragma once
#include <string>

class CImage;   // forward declaration (we don't have to include CImage.h)

class ImageFactory {
public:
    static CImage* create(const std::string& path);
};
