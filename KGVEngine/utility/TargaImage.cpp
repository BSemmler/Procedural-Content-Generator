//
// Created by Brett on 2024-03-20.
//

#include "TargaImage.h"
#include "spdlog/spdlog.h"

namespace KGV::Engine {
    bool LoadTargaFromFile(const std::string& filePath, TargaImage& image) {
        auto log = spdlog::get("engine");
        if (filePath.empty()) {
            log->error("TargaLoader, Empty Path");
            return false;
        }

        std::ifstream file(filePath, std::ios::binary);

        if (!file.is_open()) {
            log->error("File {} not found!", filePath);
            return false;
        }

        // Get the file header.
        uint8_t header[18];
//        file.read((char *) header, 18);
        spdlog::info("sizeof targa header: {}", sizeof (TargaHeader));
        file.read(reinterpret_cast<char *>(&image.tHeader), 18);
//        memcpy(&image.tHeader, header, 18);

        if (image.tHeader.descriptor & 0x10) {
            log->info("Image {} is orientated top to bottom", filePath);
            image.isTopToBottom = true;
        } else {
            log->info("Image {} is orientated bottom to top", filePath);
            image.isTopToBottom = false;
        }

        if (image.tHeader.descriptor & 0x08) {
            log->info("Image {} is orientated right to left", filePath);
            image.isRightToLeft = true;
        } else {
            log->info("Image {} is orientated left to right", filePath);
            image.isRightToLeft = false;
        }

        int length = image.tHeader.width * image.tHeader.height * (image.tHeader.bitsPerPixel / 8); // width * height * bytes per pixel
        auto data = std::make_shared<char[]>(length);
        image.data = std::make_shared<char[]>(length);
        file.read(data.get(), length);

        // We now need to reverse the image since targa stores images in an upside down format.
        //file.read( image->data.get(), length );

        image.data = data;
        //for

        return true;
    }

    void Convert24BitTo32Bit(char *in, unsigned int numPixels, unsigned int *out) {
        for (int i = 0; i < numPixels; i++) {
            auto r = static_cast<uint8_t>(in[i * 3]);
            auto g = static_cast<uint8_t>(in[i * 3 + 1]);
            auto b = static_cast<uint8_t>(in[i * 3 + 2]);
            uint8_t a = 0xFF;
            out[i] = (a << 24) | (b << 16) | (g << 8) | r;
        }
    }

    char* TargaImage::getData() {
        return data.get();
    }

    unsigned short TargaImage::getWidth() {
        return tHeader.width;
    }

    unsigned short TargaImage::getHeight() {
        return tHeader.height;
    }

    unsigned short TargaImage::getBitDepth() {
        return tHeader.bitsPerPixel;
    }

    bool TargaImage::isBottomToTop() {
        return !isTopToBottom;
    }

    bool TargaImage::isLeftToRight() {
        return !isRightToLeft;
    }
}