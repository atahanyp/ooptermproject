#include "ProcessingFactory.h"
#include "Processings.h"

CImageProcessing* ProcessingFactory::create(int toolId)
{
    switch (toolId) {
    case 1: return new CSobel();
    case 2: return new CPrewitt();
    case 3: return new CCanny();
    case 4: return new CMeanFilter();
    case 5: return new CMedianFilter();
    case 6: return new CGaussianFilter();
    case 7: return new CHistogramEqualization();
    default: return nullptr;
    }
}
