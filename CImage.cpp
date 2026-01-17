#include "CImage.h"

CImage::CImage()
    : m_w(0), m_h(0), m_c(0), m_buf(nullptr)
{
}

CImage::~CImage()
{
    freebuf();
}

int CImage::width() const
{
    return m_w;
}

int CImage::height() const
{
    return m_h;
}

int CImage::channels() const
{
    return m_c;
}

unsigned char* CImage::data()
{
    return m_buf;
}

const unsigned char* CImage::data() const
{
    return m_buf;
}

bool CImage::alloc(int w, int h, int c)
{
    if (w <= 0 || h <= 0 || c <= 0) {
        return false;
    }

    // Clean up old buffer if exists (to avoid memory leak)
    freebuf();

    // Allocate new buffer: total bytes = w*h*c
    m_buf = new unsigned char[w * h * c];

    // Save size information
    m_w = w;
    m_h = h;
    m_c = c;

    return true;
}

void CImage::freebuf()
{
    delete[] m_buf;
    m_buf = nullptr;

    m_w = 0;
    m_h = 0;
    m_c = 0;
}
