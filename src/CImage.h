#pragma once
#include <string>
class CImage {
public:
    CImage();
    virtual ~CImage();

    // Derived classes (PNG/JPEG/BMP/TIFF) will implement
    virtual bool load(std::string path) = 0;
    virtual bool save(std::string path) = 0;

    // Basic getters
    int width() const;
    int height() const;
    int channels() const;

    // Buffer access
    unsigned char* data();
    const unsigned char* data() const;

    // Buffer management
    bool alloc(int w, int h, int c);
    void freebuf();

protected:
    int m_w, m_h, m_c;
    unsigned char* m_buf;
};