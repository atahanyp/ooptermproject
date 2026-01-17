#pragma once
#include "CImageProcessing.h"

class CSobel : public CImageProcessing {
public:
    bool run() override;
};
class CPrewitt : public CImageProcessing {
public:
    bool run() override;
};
class CCanny : public CImageProcessing {
public:
    bool run() override;
};
class CMeanFilter : public CImageProcessing {
public:
    bool run() override;
};
class CMedianFilter : public CImageProcessing {
public:
    bool run() override;
};
class CGaussianFilter : public CImageProcessing {
public:
    bool run() override;
};
class CHistogramEqualization : public CImageProcessing {
public:
    bool run() override;
};
