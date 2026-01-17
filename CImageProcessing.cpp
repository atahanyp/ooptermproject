#include "CImageProcessing.h"

CImageProcessing::CImageProcessing()
    : m_in(nullptr), m_out(nullptr)
{
}

CImageProcessing::~CImageProcessing()
{
}

void CImageProcessing::setInput(CImage* in)
{
    m_in = in;
}

CImage* CImageProcessing::getInput() const
{
    return m_in;
}

CImage* CImageProcessing::getOutput() const
{
    return m_out;
}

void CImageProcessing::setOutput(CImage* out)
{
    m_out = out;
}
