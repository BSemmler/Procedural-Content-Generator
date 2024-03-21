//
// Created by Brett on 2024-03-20.
//

#ifndef ENGINE_TARGAIMAGE_H
#define ENGINE_TARGAIMAGE_H

#include "pch.h"

namespace KGV::Engine {

    class TargaImage;

    struct TargaHeader {
        unsigned char miscData[ 12 ];
        unsigned short width;
        unsigned short height;
        unsigned char bitsPerPixel;
        unsigned char descriptor;
    };

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
