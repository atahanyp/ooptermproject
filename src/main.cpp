#include <iostream>
#include <string>
#include <cstdlib>   // std::system
#include "CImage.h"
#include "ImageFactory.h"
#include "CImageProcessing.h"
#include "ProcessingFactory.h"

static std::string makeOutputPath(const std::string& inPath)
{
    size_t dot = inPath.find_last_of('.');
    if (dot == std::string::npos) return inPath + "_out";
    return inPath.substr(0, dot) + "_out" + inPath.substr(dot);
}

static void openWithDefaultViewer(const std::string& filePath)
{
#ifdef _WIN32
    std::string cmd = "start \"\" \"" + filePath + "\"";
    std::system(cmd.c_str());
#elif __APPLE__
    std::string cmd = "open \"" + filePath + "\"";
    std::system(cmd.c_str());
#else
    std::string cmd = "xdg-open \"" + filePath + "\"";
    std::system(cmd.c_str());
#endif
}

static void printMenu()
{
    std::cout
        << "\nShow processing menu:\n"
        << "  1) Sobel\n"
        << "  2) Prewitt\n"
        << "  3) Canny\n"
        << "  4) Noise Removal: Mean\n"
        << "  5) Noise Removal: Median\n"
        << "  6) Noise Removal: Gaussian\n"
        << "  7) Histogram Equalization\n"
        << "  E) Exit\n";
}

int main()
{
    std::cout << "Enter path" << std::endl;
    std::string path;
    std::cin >> path;
    std::string originalPath = path;

    if (path.empty()) {
        std::cerr << "Error: empty path.\n";
        return 1;
    }

    // Factory creates appropriate loader, loads image
    CImage* current = ImageFactory::create(path);
    if (current == nullptr) {
        std::cerr << "Error: unsupported format or factory failed.\n";
        return 1;
    }

    if (!current->load(path)) {
        std::cerr << "Error: failed to load image: " << path << "\n";
        delete current;
        return 1;
    }

    std::cout << "Loaded: " << path
              << " (" << current->width() << "x" << current->height()
              << ", channels=" << current->channels() << ")\n";

    // Driver loop
    while (true) {
        printMenu();
        std::cout << "Select a tool: ";
        std::string choice;
        std::cin >> choice;

        // Exit condition (E/e)
        if (choice == "E" || choice == "e") {
            break;
        }

        // Parse 1..7
        int toolId = -1;
        if (!choice.empty() && choice[0] >= '1' && choice[0] <= '7') {
            toolId = choice[0] - '0';
        } else {
            std::cout << "Invalid choice.\n";
            continue;
        }

        // Create processing tool via factory (driver does NOT new derived tools)
        CImageProcessing* proc = ProcessingFactory::create(toolId);
        if (!proc) {
            std::cout << "Tool creation failed.\n";
            continue;
        }

        proc->setInput(current);

        if (!proc->run()) {
            std::cout << "Tool run failed.\n";
            delete proc;
            continue;
        }

        CImage* out = proc->getOutput();
        delete proc;

        if (!out) {
            std::cout << "Tool produced null output.\n";
            continue;
        }

        // Output saved automatically as inputFile_out.[ext]
        std::string outPath = makeOutputPath(originalPath);

        if (!out->save(outPath)) {
            std::cout << "Save failed.\n";
            delete out;
            continue;
        }

        // Open with default image viewer (OS default app)
        openWithDefaultViewer(outPath);

        // Replace current with output
        delete current;
        current = out;
        path = outPath;

        std::cout << "Saved output: " << outPath << "\n";
    }

    delete current;
    std::cout << "closing program ...\n";
    return 0;
}
