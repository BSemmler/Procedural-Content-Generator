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

    typedef std::function<double(double, double, double, int, int)> NoiseOp;
    typedef std::function<double(double, double, double, double, int, int)> CombineNoiseOp;

    class NoiseBufferGenerator {
    public:
        NoiseBufferGenerator();
        NoiseBufferGenerator(unsigned int threadCount);
        void generateNoiseTexture2D(fBmConfig& fBmConf, float* buffer, double xPosOffset = 0, double yPosOffset = 0, int width = 256, int height = 256);
        void generateNoiseTexture2D(fBmConfig& fBmConf, double* buffer, double xPosOffset = 0, double yPosOffset = 0, int width = 256, int height = 256);
//        void generateNoise3DSP(fBmConfig& fBm, void* buffer, int stride, int memOffset, double xPosOffset = 0, double yPosOffset = 0, double zPosOffset = 0, int width = 256, int height = 256, int depth = 256);
//        void generateNoise3DDP(fBmConfig& fBm, void* buffer, int stride, int memOffset, double xPosOffset = 0, double yPosOffset = 0, double zPosOffset = 0, int width = 256, int height = 256, int depth = 256);

        void execOp(float* buffer, int width, int height, const NoiseOp& func);
        void execOp(double* buffer, int width, int height, const NoiseOp& func);
        void combine(double *a, double *b, double *out, int width, int height, const CombineNoiseOp& func);

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