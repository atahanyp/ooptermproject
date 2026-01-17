#pragma once
#include "CImage.h"

// PNG
class CPngImage : public CImage {
public:
    bool load(std::string path) override;
    bool save(std::string path) override;
};

// JPEG
class CJpegImage : public CImage {
public:
    bool load(std::string path) override;
    bool save(std::string path) override;
};

// BMP
class CBmpImage : public CImage {
public:
    bool load(std::string path) override;
    bool save(std::string path) override;
};

// TIFF
class CTiffImage : public CImage {
public:
    bool load(std::string path) override;
    bool save(std::string path) override;
};
