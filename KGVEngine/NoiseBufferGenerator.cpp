//
// Created by Brett on 2024-03-16.
//

#include "NoiseBufferGenerator.h"
namespace KGV::Procedural {


    void NoiseBufferGenerator::generateNoiseTexture2DSP(fBmConfig& fBmConf, void *buffer, int stride, int memOffset,
                                                        int xPosOffset, int yPosOffset, int width, int height) {
        const unsigned int totalElements = width * height;
        const unsigned int elementsPerThread = (height / maxThreads) * width;

        for (int i = 0; i < maxThreads; ++i) {
            unsigned int begin = i * elementsPerThread;
            unsigned int end = (i == maxThreads - 1) ? totalElements : (i + 1) * elementsPerThread;
            auto t = (char*)buffer + (stride * begin);
            spdlog::info("Start: {}, End: {}, Address: {}", begin, end, t);

            threadPool.emplace_back(([this, &fBmConf, width, height, stride, memOffset, xPosOffset, yPosOffset]
            (int begin, int end, void* buffer) {
                for (int i = begin; i < end; ++i) {
                    double xf = i % width;
                    double yf = i / width;
                    auto val = static_cast<float>(fBm(fBmConf, xf / width + xPosOffset, yf / height + yPosOffset));
                    memcpy((char*)buffer + memOffset, &val, sizeof(float));

                    // Need
                    auto t = (char*)buffer;
                    t += stride;
                    buffer = t;
                }

            }), begin, end, (char*)buffer + (stride * begin));
        }

        for (auto &t : threadPool) {
            t.join();
        }
    }

    void NoiseBufferGenerator::generateNoiseTexture2DDP(fBmConfig& fBmConf, void *buffer, int stride, int memOffset,
                                                        int xPosOffset, int yPosOffset, int width, int height) {
        const unsigned int totalElements = width * height;
        const unsigned int elementsPerThread = (height / maxThreads) * width;

        for (int i = 0; i < maxThreads; ++i) {
            unsigned int begin = i * elementsPerThread;
            unsigned int end = (i == maxThreads - 1) ? totalElements : (i + 1) * elementsPerThread;
            auto t = (char*)buffer + (stride * begin);
            spdlog::info("Start: {}, End: {}, Address: {}", begin, end, (void*)t);

            threadPool.emplace_back(([this, &fBmConf, width, height, stride, memOffset, xPosOffset, yPosOffset]
                    (int begin, int end, void* buffer) {
                for (int i = begin; i < end; ++i) {
                    double xf = i % width;
                    double yf = i / width;
//                    spdlog::info("xf {} yf {}", xf, yf);
                    auto val = fBm(fBmConf, xf / width + xPosOffset, yf / height + yPosOffset);
                    memcpy((char*)buffer + memOffset, &val, sizeof(double));
//                    spdlog::info("xf {} yf {} val {}", xf, yf, val);
                    // Need
                    auto t = (char*)buffer;
                    t += stride;
                    buffer = t;
                }

            }), begin, end, (char*)buffer + (stride * begin));
        }

        for (auto &t : threadPool) {
            t.join();
        }
    }

//    void NoiseBufferGenerator::generateNoise3DSP(fBmConfig& fBm, void *buffer, int stride, int memOffset, int xPosOffset, int yPosOffset,
//                                                 int zPosOffset, int width, int height, int depth) {
//
//    }
//
//    void NoiseBufferGenerator::generateNoise3DDP(fBmConfig& fBm, void *buffer, int stride, int memOffset, int xPosOffset, int yPosOffset,
//                                                                  int zPosOffset, int width, int height, int depth) {
//
//    }

    double NoiseBufferGenerator::octaveNoise(double x, double y, double z, double frequency, double amplitude) {
        return noiseGen.noiseDP(x * frequency, y * frequency, z * frequency) * amplitude;
    }

    double NoiseBufferGenerator::octaveNoise(double x, double y, double frequency, double amplitude) {
        return noiseGen.noiseDP(x * frequency, y * frequency) * amplitude;
    }

    NoiseBufferGenerator::NoiseBufferGenerator() {
        maxThreads = std::max<unsigned int>(std::thread::hardware_concurrency(), 4);
    }

    NoiseBufferGenerator::NoiseBufferGenerator(unsigned int _threadCount) {
        maxThreads = _threadCount;
    }

    double NoiseBufferGenerator::fBm(fBmConfig &fBmConf, double x, double y) {
        auto freq = fBmConf.frequency;
        auto amplitude = fBmConf.amplitude;

        double total = 0;
        double totalAmplitude = 0;
        for (int i = 0; i < fBmConf.octaves; ++i) {
            total += octaveNoise(x, y, freq, amplitude);
            totalAmplitude += amplitude;
            freq *= fBmConf.lacunarity;
            amplitude *= fBmConf.persistence;
        }

        return total / totalAmplitude;
    }

}

