//
// Created by Brett on 2024-03-16.
//

#include "NoiseBufferGenerator.h"
namespace KGV::Procedural {


    void NoiseBufferGenerator::generateNoiseTexture2D(fBmConfig& fBmConf, float *buffer,
                                                        double xPosOffset, double yPosOffset, int width, int height) {
        const unsigned int totalElements = width * height;
        const unsigned int elementsPerThread = (height / maxThreads) * width;

        for (int i = 0; i < maxThreads; ++i) {
            unsigned int begin = i * elementsPerThread;
            unsigned int end = (i == maxThreads - 1) ? totalElements : (i + 1) * elementsPerThread;

            threadPool.emplace_back(([this, &fBmConf, width, height, xPosOffset, yPosOffset]
            (int begin, int end, float* buffer) {
                for (int i = begin; i < end; ++i) {
                    double xf = i % width;
                    double yf = i / width;
                    auto val = static_cast<float>(fBm(fBmConf, xf / width + xPosOffset, yf / height + yPosOffset));
                    buffer[i] = val;
                }

            }), begin, end, buffer);
        }

        for (auto &t : threadPool) {
            t.join();
        }

        threadPool.clear();
    }

    void NoiseBufferGenerator::generateNoiseTexture2D(fBmConfig& fBmConf, double *buffer,
                                                        double xPosOffset, double yPosOffset, int width, int height) {
        const unsigned int totalElements = width * height;
        const unsigned int elementsPerThread = (height / maxThreads) * width;

        for (int i = 0; i < maxThreads; ++i) {
            unsigned int begin = i * elementsPerThread;
            unsigned int end = (i == maxThreads - 1) ? totalElements : (i + 1) * elementsPerThread;

            threadPool.emplace_back(([this, &fBmConf, width, height, xPosOffset, yPosOffset]
                    (int begin, int end, double* buffer) {
                for (int i = begin; i < end; ++i) {
                    double xf = i % width;
                    double yf = i / width;

                    auto val = fBm(fBmConf, xf / width + xPosOffset, yf / height + yPosOffset);
                    buffer[i] = val;
                }

            }), begin, end, buffer);
        }

        for (auto &t : threadPool) {
            t.join();
        }

        threadPool.clear();
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

    void NoiseBufferGenerator::execOp(float *buffer, int width, int height, const NoiseOp &func) {
        const unsigned int totalElements = width * height;
        const unsigned int elementsPerThread = (height / maxThreads) * width;

        for (int i = 0; i < maxThreads; ++i) {
            unsigned int begin = i * elementsPerThread;
            unsigned int end = (i == maxThreads - 1) ? totalElements : (i + 1) * elementsPerThread;

            threadPool.emplace_back(([width, height, func](int begin, int end, float* buffer) {
                for (int i = begin; i < end; ++i) {
                    auto xf = static_cast<double>(i % width);
                    auto yf =  static_cast<double>(i / width);
                    auto val = static_cast<float>(func(*buffer, xf, yf, width, height));

                    memcpy(buffer, &val, sizeof(float));
                    buffer++;
                }

            }), begin, end, buffer + begin);
        }

        for (auto &t : threadPool) {
            t.join();
        }

        threadPool.clear();
    }

    void NoiseBufferGenerator::execOp(double *buffer, int width, int height, const NoiseOp& func) {
        const unsigned int totalElements = width * height;
        const unsigned int elementsPerThread = (height / maxThreads) * width;

        for (int i = 0; i < maxThreads; ++i) {
            unsigned int begin = i * elementsPerThread;
            unsigned int end = (i == maxThreads - 1) ? totalElements : (i + 1) * elementsPerThread;

            threadPool.emplace_back(([width, height, func](int begin, int end, double* buffer) {
                for (int i = begin; i < end; ++i) {
                    auto xf = static_cast<double>(i % width);
                    auto yf =  static_cast<double>(i / width);
                    auto val = func(*buffer, xf, yf, width, height);

                    memcpy(buffer, &val, sizeof(double));
                    buffer++;
                }

            }), begin, end, buffer + begin);
        }

        for (auto &t : threadPool) {
            t.join();
        }

        threadPool.clear();
    }

    void NoiseBufferGenerator::combine(double *a, double *b, double *out, int width, int height,
                                       const CombineNoiseOp& func) {

        const unsigned int totalElements = width * height;
        const unsigned int elementsPerThread = (height / maxThreads) * width;

        for (int i = 0; i < maxThreads; ++i) {
            unsigned int begin = i * elementsPerThread;
            unsigned int end = (i == maxThreads - 1) ? totalElements : (i + 1) * elementsPerThread;

            auto inA = a + begin;
            auto inB = b + begin;
            auto inOut = out + begin;

            threadPool.emplace_back(([width, height, func](int begin, int end, double* a, double* b, double* out) {
                for (int i = begin; i < end; ++i) {
                    auto xf = static_cast<double>(i % width);
                    auto yf =  static_cast<double>(i / width);
                    auto val = func(*a, *b, xf, yf, width, height);

                    memcpy(out, &val, sizeof(double));
                    a++;
                    b++;
                    out++;
                }

            }), begin, end, inA, inB, inOut);
        }

        for (auto &t : threadPool) {
            t.join();
        }
    }



}

