#include "Processings.h"
#include "CImage.h"
#include "ImageFactory.h"

#include <opencv2/imgproc.hpp>
#include <opencv2/imgcodecs.hpp>

#include <iostream>


static cv::Mat wrapMat(CImage* img)
{
    int w = img->width();
    int h = img->height();
    int c = img->channels();
    int type = (c == 1) ? CV_8UC1 : (c == 3 ? CV_8UC3 : CV_8UC4);
    return cv::Mat(h, w, type, img->data());
}

// Produce output in the same format as input (for now, same derived is chosen as "out.png")
static CImage* makeOutLikeInput(CImage* in)
{
    CImage* out = ImageFactory::create("out.png");
    if (!out) return nullptr;
    if (!out->alloc(in->width(), in->height(), in->channels())) {
        delete out;
        return nullptr;
    }
    return out;
}

// If color (3/4 channels), convert to Gray, return 1 channel.
// return: gray Mat (CV_8UC1)
static cv::Mat toGray(const cv::Mat& src)
{
    if (src.channels() == 1) return src;

    cv::Mat gray;
    if (src.channels() == 3) cv::cvtColor(src, gray, cv::COLOR_RGB2GRAY);
    else if (src.channels() == 4) cv::cvtColor(src, gray, cv::COLOR_RGBA2GRAY);
    else gray = src; // unexpected but safe
    return gray;
}


bool CSobel::run()
{
    CImage* in = getInput();
    if (!in) return false;

    std::cout << "[RUN] Sobel (magnitude)\n";

    cv::Mat src = wrapMat(in);
    cv::Mat gray = toGray(src);

    cv::Mat gx, gy;
    cv::Sobel(gray, gx, CV_16S, 1, 0, 3);
    cv::Sobel(gray, gy, CV_16S, 0, 1, 3);

    cv::Mat absx, absy, mag;
    cv::convertScaleAbs(gx, absx);
    cv::convertScaleAbs(gy, absy);
    cv::addWeighted(absx, 0.5, absy, 0.5, 0, mag); // simple magnitude

    // output: 1 channel
    CImage* out = ImageFactory::create("out.png");
    if (!out) return false;
    if (!out->alloc(in->width(), in->height(), 1)) { delete out; return false; }

    std::memcpy(out->data(), mag.data, (size_t)in->width() * in->height());

    setOutput(out);
    return true;
}

bool CPrewitt::run()
{
    CImage* in = getInput();
    if (!in) return false;

    std::cout << "[RUN] Prewitt (magnitude)\n";

    cv::Mat src = wrapMat(in);
    cv::Mat gray = toGray(src);

    cv::Mat kx = (cv::Mat_<float>(3,3) <<
        -1, 0, 1,
        -1, 0, 1,
        -1, 0, 1);

    cv::Mat ky = (cv::Mat_<float>(3,3) <<
         1,  1,  1,
         0,  0,  0,
        -1, -1, -1);

    cv::Mat fx, fy;
    cv::filter2D(gray, fx, CV_32F, kx);
    cv::filter2D(gray, fy, CV_32F, ky);

    cv::Mat ax, ay, mag;
    cv::convertScaleAbs(fx, ax);
    cv::convertScaleAbs(fy, ay);
    cv::addWeighted(ax, 0.5, ay, 0.5, 0, mag);

    CImage* out = ImageFactory::create("out.png");
    if (!out) return false;
    if (!out->alloc(in->width(), in->height(), 1)) { delete out; return false; }

    std::memcpy(out->data(), mag.data, (size_t)in->width() * in->height());

    setOutput(out);
    return true;
}

bool CCanny::run()
{
    CImage* in = getInput();
    if (!in) return false;

    std::cout << "[RUN] Canny (50, 150)\n";

    cv::Mat src = wrapMat(in);
    cv::Mat gray = toGray(src);

    cv::Mat edges;
    cv::Canny(gray, edges, 50, 150);

    CImage* out = ImageFactory::create("out.png");
    if (!out) return false;
    if (!out->alloc(in->width(), in->height(), 1)) { delete out; return false; }

    std::memcpy(out->data(), edges.data, (size_t)in->width() * in->height());

    setOutput(out);
    return true;
}

bool CMeanFilter::run()
{
    CImage* in = getInput();
    if (!in) return false;

    std::cout << "[RUN] Mean Filter (3x3)\n";

    CImage* out = makeOutLikeInput(in);
    if (!out) return false;

    cv::Mat src = wrapMat(in);
    cv::Mat dst = wrapMat(out);

    cv::blur(src, dst, cv::Size(3, 3));

    setOutput(out);
    return true;
}


bool CMedianFilter::run()
{
    CImage* in = getInput();
    if (!in) return false;

    std::cout << "[RUN] Median Filter (ksize=3)\n";

    CImage* out = makeOutLikeInput(in);
    if (!out) return false;

    cv::Mat src = wrapMat(in);
    cv::Mat dst = wrapMat(out);

    cv::medianBlur(src, dst, 3);

    setOutput(out);
    return true;
}


bool CGaussianFilter::run()
{
    CImage* in = getInput();
    if (!in) return false;

    std::cout << "[RUN] Gaussian Filter (5x5, sigma=1.0)\n";

    CImage* out = makeOutLikeInput(in);
    if (!out) return false;

    cv::Mat src = wrapMat(in);
    cv::Mat dst = wrapMat(out);

    cv::GaussianBlur(src, dst, cv::Size(5, 5), 1.0);

    setOutput(out);
    return true;
}

bool CHistogramEqualization::run()
{
    CImage* in = getInput();
    if (!in) return false;

    std::cout << "[RUN] Histogram Equalization\n";

    cv::Mat src = wrapMat(in);

    // If 1 channel, use directly
    if (src.channels() == 1)
    {
        cv::Mat eq;
        cv::equalizeHist(src, eq);

        CImage* out = ImageFactory::create("out.png");
        if (!out) return false;
        if (!out->alloc(in->width(), in->height(), 1)) { delete out; return false; }

        std::memcpy(out->data(), eq.data, (size_t)in->width() * in->height());
        setOutput(out);
        return true;
    }

    // 3/4 channels: RGB(A) -> YCrCb (if alpha exists, convert to RGB first)
    cv::Mat rgb = src;
    cv::Mat work;

    if (src.channels() == 4) {
        cv::cvtColor(src, work, cv::COLOR_RGBA2RGB);
        rgb = work;
    }

    cv::Mat ycrcb;
    cv::cvtColor(rgb, ycrcb, cv::COLOR_RGB2YCrCb);

    std::vector<cv::Mat> ch;
    cv::split(ycrcb, ch);

    cv::equalizeHist(ch[0], ch[0]); // Y channel

    cv::merge(ch, ycrcb);

    cv::Mat outRgb;
    cv::cvtColor(ycrcb, outRgb, cv::COLOR_YCrCb2RGB);

    // output channels: 3 if input is 3, 4 if input is 4 (requires extra work to preserve alpha)
    // For now: keep alpha preservation simple for 4 channels: copy alpha from input.
    CImage* out = ImageFactory::create("out.png");
    if (!out) return false;

    if (src.channels() == 3)
    {
        if (!out->alloc(in->width(), in->height(), 3)) { delete out; return false; }
        std::memcpy(out->data(), outRgb.data, (size_t)in->width() * in->height() * 3);
    }
    else // 4 kanal
    {
        if (!out->alloc(in->width(), in->height(), 4)) { delete out; return false; }

        // Write RGB, copy alpha from input
        unsigned char* outBuf = out->data();
        const unsigned char* inBuf = in->data();

        int pixels = in->width() * in->height();
        for (int i = 0; i < pixels; ++i) {
            outBuf[4*i + 0] = outRgb.data[3*i + 0];
            outBuf[4*i + 1] = outRgb.data[3*i + 1];
            outBuf[4*i + 2] = outRgb.data[3*i + 2];
            outBuf[4*i + 3] = inBuf[4*i + 3]; // alpha channel
        }
    }

    setOutput(out);
    return true;
}

