//
// Created by Brett on 2024-03-16.
//

#ifndef ENGINE_NOISEBUFFERGENERATOR_H
#define ENGINE_NOISEBUFFERGENERATOR_H


#include "pch.h"
#include "PerlinNoise.h"

namespace KGV::Procedural {
    struct fBmConfig {
        double frequency;
        double amplitude;
        double persistence;
        double lacunarity;
        int octaves;
    };

    class NoiseBufferGenerator {
    public:
        NoiseBufferGenerator();
        NoiseBufferGenerator(unsigned int threadCount);
        void generateNoiseTexture2DSP(fBmConfig& fBmConf, void* buffer, int stride, int memOffset, int xPosOffset = 0, int yPosOffset = 0, int width = 256, int height = 256);
        void generateNoiseTexture2DDP(fBmConfig& fBmConf, void* buffer, int stride, int memOffset, int xPosOffset = 0, int yPosOffset = 0, int width = 256, int height = 256);
//        void generateNoise3DSP(fBmConfig& fBm, void* buffer, int stride, int memOffset, int xPosOffset = 0, int yPosOffset = 0, int zPosOffset = 0, int width = 256, int height = 256, int depth = 256);
//        void generateNoise3DDP(fBmConfig& fBm, void* buffer, int stride, int memOffset, int xPosOffset = 0, int yPosOffset = 0, int zPosOffset = 0, int width = 256, int height = 256, int depth = 256);
    protected:
        double fBm(fBmConfig& fBmConf, double x, double y);
        double octaveNoise(double x, double y, double z, double frequency, double amplitude);
        double octaveNoise(double x, double y, double frequency, double amplitude);
        unsigned int threadCount;

        PerlinNoise noiseGen;
        unsigned int maxThreads;
        std::vector<std::thread> threadPool;
    };
}



#endif //ENGINE_NOISEBUFFERGENERATOR_H
