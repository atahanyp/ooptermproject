#include "Images.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>

static bool fakeLoad(CImage* img, const std::string& path)
{
    // Pretend to load: 640x480 RGB
    if (!img->alloc(640, 480, 3)) return false;

    std::cout << "[STUB] Loaded " << path << " as 640x480x3\n";
    return true;
}

static bool fakeSave(const std::string& path)
{
    std::cout << "[STUB] Saved to " << path << "\n";
    return true;
}
static bool loadWithOpenCV(CImage* img, const std::string& path)
{
    // 1) Read file (OpenCV)
    cv::Mat m = cv::imread(path, cv::IMREAD_UNCHANGED);
    if (m.empty()) {
        std::cerr << "OpenCV imread failed: " << path << "\n";
        return false;
    }

    // 2) Convert to 8-bit if not already (simple/safe approach)
    if (m.depth() != CV_8U) {
        cv::Mat tmp;
        m.convertTo(tmp, CV_8U);
        m = tmp;
    }

    // 3) Width and number of channels
    int w = m.cols;
    int h = m.rows;
    int c = m.channels();

    // 4) Let's keep our internal buffer in RGB/RGBA:
    // OpenCV reads BGR/BGRA by default.
    if (c == 3) cv::cvtColor(m, m, cv::COLOR_BGR2RGB);
    else if (c == 4) cv::cvtColor(m, m, cv::COLOR_BGRA2RGBA);

    // 5) Allocate our buffer
    if (!img->alloc(w, h, c)) return false;

    // 6) Clone if Mat memory is not continuous, then copy
    if (!m.isContinuous()) m = m.clone();

    std::memcpy(img->data(), m.data, (size_t)w * h * c);
    return true;
}


static bool saveWithOpenCV(const CImage* img, const std::string& path)
{
    int w = img->width();
    int h = img->height();
    int c = img->channels();

    if (!img->data() || w <= 0 || h <= 0 || (c != 1 && c != 3 && c != 4)) {
        std::cerr << "Invalid image buffer for save.\n";
        return false;
    }

    // 1) Wrap our buffer as Mat
    int type = (c == 1) ? CV_8UC1 : (c == 3 ? CV_8UC3 : CV_8UC4);
    cv::Mat m(h, w, type, const_cast<unsigned char*>(img->data()));

    // 2) If internal is RGB, convert to BGR before OpenCV writes
    cv::Mat out = m;
    if (c == 3) cv::cvtColor(m, out, cv::COLOR_RGB2BGR);
    else if (c == 4) cv::cvtColor(m, out, cv::COLOR_RGBA2BGRA);

    // 3) Write
    if (!cv::imwrite(path, out)) {
        std::cerr << "OpenCV imwrite failed: " << path << "\n";
        return false;
    }
    return true;
}


bool CPngImage::load(std::string path)  { std::cout << "[FACTORY] CPngImage selected\n";  return loadWithOpenCV(this, path); }
bool CPngImage::save(std::string path)  { return saveWithOpenCV(this, path); }

bool CJpegImage::load(std::string path) { return loadWithOpenCV(this, path); }
bool CJpegImage::save(std::string path) { return saveWithOpenCV(this, path); }

bool CBmpImage::load(std::string path)  { return loadWithOpenCV(this, path); }
bool CBmpImage::save(std::string path)  { return saveWithOpenCV(this, path); }

bool CTiffImage::load(std::string path) { return loadWithOpenCV(this, path); }
bool CTiffImage::save(std::string path) { return saveWithOpenCV(this, path); }