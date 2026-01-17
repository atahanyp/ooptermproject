#include "ImageFactory.h"
#include "CImage.h"

// For now, I assume derived class names like this:
#include "Images.h"


static bool endsWith(const std::string& s, const std::string& suffix)
{
    if (s.size() < suffix.size()) return false;
    return s.compare(s.size() - suffix.size(), suffix.size(), suffix) == 0;
}

CImage* ImageFactory::create(const std::string& path)
{
    // Select based on file extension
    if (endsWith(path, ".png"))  return new CPngImage();
    if (endsWith(path, ".jpg"))  return new CJpegImage();
    if (endsWith(path, ".jpeg")) return new CJpegImage();
    if (endsWith(path, ".bmp"))  return new CBmpImage();
    if (endsWith(path, ".tif"))  return new CTiffImage();
    if (endsWith(path, ".tiff")) return new CTiffImage();

    return nullptr; // unknown format
}
