#pragma once

class CImageProcessing;

class ProcessingFactory {
public:
    static CImageProcessing* create(int toolId);
};
