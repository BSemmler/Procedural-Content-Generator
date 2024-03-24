//
// Created by Brett on 2024-03-16.
//

#ifndef ENGINE_MULTITHREADEDNOISEEXECUTOR_H
#define ENGINE_MULTITHREADEDNOISEEXECUTOR_H


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

    enum eBufferFormat {
        kR8G8B8 = 0,
        kR8G8B8A8 = 1,
        kR32G32B32 = 2,
        kR32G32B32A32 = 3
    };

    typedef std::function<double(double, double, double, int, int)> NoiseOp;
    typedef std::function<unsigned int(double, double, double, int, int)> ImageOp;
    typedef std::function<double(double, double, double, double, int, int)> Combine2NoiseOp;
    typedef std::function<double(double, double, double, double, double, int, int)> Combine3NoiseOp;

    class MultithreadedNoiseExecutor {
    public:
        MultithreadedNoiseExecutor();
        MultithreadedNoiseExecutor(unsigned int numThreads);
        void execOp(float* buffer, int width, int height, const NoiseOp& func);
        void execOp(double* buffer, int width, int height, const NoiseOp& func);
        void combine(double *a, double *b, double *out, int width, int height, const Combine2NoiseOp& func);
        void combine(float *a, float *b, float *out, int width, int height, const Combine2NoiseOp& func);
        void combine(float *a, float *b, float *c, float *out, int width, int height, const Combine3NoiseOp& func);
        void createPixelBufferFromData(unsigned int* out, float* in, int width, int height, ImageOp& func);

    protected:
        unsigned int maxThreads;
        std::vector<std::thread> threadPool;
    };
}



#endif //ENGINE_MULTITHREADEDNOISEEXECUTOR_H
