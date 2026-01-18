cmake_minimum_required(VERSION 3.10)
project(ImageProcessingTool VERSION 1.0)

# C++ Standard
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Find OpenCV
find_package(OpenCV REQUIRED COMPONENTS core imgproc imgcodecs)

# Include directories
include_directories(${OpenCV_INCLUDE_DIRS})

# Source files
set(SOURCES
    src/main.cpp
    src/CImage.cpp
    src/ImageFactory.cpp
    src/Images.cpp
    src/CImageProcessing.cpp
    src/ProcessingFactory.cpp
    src/Processings.cpp
)

# Header files
set(HEADERS
    src/CImage.h
    src/ImageFactory.h
    src/Images.h
    src/CImageProcessing.h
    src/ProcessingFactory.h
    src/Processings.h
)

# Executable
add_executable(main ${SOURCES} ${HEADERS})

# Link OpenCV libraries
target_link_libraries(main ${OpenCV_LIBS})

# Output directory
set_target_properties(main PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}
)
