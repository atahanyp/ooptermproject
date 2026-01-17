#pragma once
#include <string>

class CImage;

class CImageProcessing {
public:
    CImageProcessing();
    virtual ~CImageProcessing();

    void setInput(CImage* in);
    CImage* getInput() const;      // <-- 
    CImage* getOutput() const;
    void setOutput(CImage* out);

    // Each tool will implement this
    virtual bool run() = 0;

protected:
    CImage* m_in;
    CImage* m_out;
};
