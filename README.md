# Image Processing Tool

A C++ command-line application for applying various image processing operations using OpenCV. Built with object-oriented design patterns.

**Note**: This project was developed as a term project for an OOP lecture to learn C++ and object-oriented programming principles.

## Features

The application supports the following image processing operations:
- **Edge Detection**: Sobel, Prewitt, Canny
- **Noise Removal**: Mean Filter, Median Filter, Gaussian Filter
- **Enhancement**: Histogram Equalization

**Supported formats**: PNG, JPEG, BMP, TIFF

## Project Structure

The project uses the Factory pattern for extensibility:

- **CImage**: Abstract base class for image types
  - `CPngImage`, `CJpegImage`, `CBmpImage`, `CTiffImage` - Format-specific implementations
- **ImageFactory**: Creates appropriate image loader based on file extension
- **CImageProcessing**: Abstract base class for processing operations
  - `CSobel`, `CPrewitt`, `CCanny`, `CMeanFilter`, etc. - Specific operations
- **ProcessingFactory**: Creates processing tools by ID
- **main.cpp**: Interactive menu-driven interface

## Build & Run

**Prerequisites**: OpenCV, MSYS2/UCRT64 (Windows)

**Build**:
```bash
g++ -g -IC:/msys64/ucrt64/include/opencv4 -LC:/msys64/ucrt64/lib \
    main.cpp CImage.cpp ImageFactory.cpp Images.cpp \
    CImageProcessing.cpp ProcessingFactory.cpp Processings.cpp \
    -o main.exe -lopencv_core -lopencv_imgproc -lopencv_imgcodecs
```

Or use the VS Code build task: `C/C++: build project (OpenCV)`

**Run**:
```bash
./main.exe
```

Enter an image path, then select operations from the menu. Processed images are saved as `filename_out.ext` and opened automatically.

