//
// Created by Brett on 2024-03-20.
//

#ifndef ENGINE_TARGAIMAGE_H
#define ENGINE_TARGAIMAGE_H

#include "pch.h"

namespace KGV::Engine {

    class TargaImage;

    enum eTargaColourMapType : uint8_t {
        kNoImageData = 0,
        kUncompressColorMapped = 1,
        kUncompressedRGB = 2,
        kUncompressBlackAndWhite = 3,
        kRunlengthEncodedColorMapped = 9,
        kRunlengthEncodedRGB = 9,
        kCompressBlackAndWhite = 11,
        kCompressed1 = 32,
        kCompressed2 = 33
    };

    // Disable padding in buffer.
#ifdef _WIN32
#pragma pack(push, 1)
    struct TargaHeader {
        uint8_t idLength;
        uint8_t colorMapType;
        uint8_t dataTypeCode;
        uint16_t colorMapOrigin;
        uint16_t colorMapLength;
        uint8_t colorMapDepth;
        uint16_t xOrigin;
        uint16_t yOrigin;
        uint16_t width;
        uint16_t height;
        uint8_t bitsPerPixel;
        uint8_t descriptor;
    };
#pragma pack(pop)
#elif
    #pragma pack(1)
    struct TargaHeader {
        uint8_t idLength;
        uint8_t colorMapType;
        uint8_t dataTypeCode;
        uint16_t colorMapOrigin;
        uint16_t colorMapLength;
        uint8_t colorMapDepth;
        uint16_t xOrigin;
        uint16_t yOrigin;
        uint16_t width;
        uint16_t height;
        uint8_t bitsPerPixel;
        uint8_t descriptor;
    };
#pragma pack()
#endif

    bool LoadTargaFromFile( const std::string& filePath, TargaImage& image );
    void Convert24BitTo32Bit(char* in, unsigned int numPixels, unsigned int* out);

    class TargaImage
    {
    public:
        char* getData();
        unsigned short getWidth();
        unsigned short getHeight();
        unsigned short getBitDepth();

        bool isBottomToTop();
        bool isLeftToRight();

    protected:
        TargaHeader tHeader;
        std::shared_ptr<char[]> data;
        bool isTopToBottom;
        bool isRightToLeft;

        friend bool LoadTargaFromFile( const std::string& filePath, TargaImage& image );
    };
}



#endif //ENGINE_TARGAIMAGE_H
