//
// Created by Brett on 2024-03-16.
//

#include "MultithreadedNoiseExecutor.h"
namespace KGV::Procedural {
    MultithreadedNoiseExecutor::MultithreadedNoiseExecutor() {
        maxThreads = std::max<unsigned int>(std::thread::hardware_concurrency(), 4);
    }

    MultithreadedNoiseExecutor::MultithreadedNoiseExecutor(unsigned int _threadCount) {
        maxThreads = _threadCount;
    }

    void MultithreadedNoiseExecutor::execOp(float *buffer, int width, int height, const NoiseOp &func) {
        const unsigned int totalElements = width * height;
        const unsigned int elementsPerThread = (height / maxThreads) * width;

        for (int i = 0; i < maxThreads; ++i) {
            unsigned int begin = i * elementsPerThread;
            unsigned int end = (i == maxThreads - 1) ? totalElements : (i + 1) * elementsPerThread;

            threadPool.emplace_back(([width, height, func](int begin, int end, float* buffer) {
                float min = 1000;
                float max = -1000;
                for (int i = begin; i < end; ++i) {
                    auto xf = static_cast<double>(i % width);
                    auto yf =  static_cast<double>(i / width);
                    auto val = static_cast<float>(func(buffer[i], xf, yf, width, height));

                    buffer[i] = val;
                }
            }), begin, end, buffer);
        }

        for (auto &t : threadPool) {
            t.join();
        }

        threadPool.clear();
    }

    void MultithreadedNoiseExecutor::execOp(double *buffer, int width, int height, const NoiseOp& func) {
        const unsigned int totalElements = width * height;
        const unsigned int elementsPerThread = (height / maxThreads) * width;

        for (int i = 0; i < maxThreads; ++i) {
            unsigned int begin = i * elementsPerThread;
            unsigned int end = (i == maxThreads - 1) ? totalElements : (i + 1) * elementsPerThread;

            threadPool.emplace_back(([width, height, func](int begin, int end, double* buffer) {
                for (int i = begin; i < end; ++i) {
                    auto xf = static_cast<double>(i % width);
                    auto yf =  static_cast<double>(i / width);
                    auto val = func(buffer[i], xf, yf, width, height);
                    buffer[i] = val;
                }

            }), begin, end, buffer);
        }

        for (auto &t : threadPool) {
            t.join();
        }

        threadPool.clear();
    }

    void MultithreadedNoiseExecutor::combine(double *a, double *b, double *out, int width, int height,
                                             const Combine2NoiseOp& func) {

        const unsigned int totalElements = width * height;
        const unsigned int elementsPerThread = (height / maxThreads) * width;

        for (int i = 0; i < maxThreads; ++i) {
            unsigned int begin = i * elementsPerThread;
            unsigned int end = (i == maxThreads - 1) ? totalElements : (i + 1) * elementsPerThread;

            threadPool.emplace_back(([width, height, func](int begin, int end, double* a, double* b, double* out) {
                for (int i = begin; i < end; ++i) {
                    auto xf = static_cast<double>(i % width);
                    auto yf =  static_cast<double>(i / width);
                    out[i] = func(a[i], b[i], xf, yf, width, height);
                }

            }), begin, end, a, b, out);
        }

        for (auto &t : threadPool) {
            t.join();
        }

        threadPool.clear();
    }

    void MultithreadedNoiseExecutor::combine(float *a, float *b, float *out, int width, int height,
                                             const Combine2NoiseOp& func) {

        const unsigned int totalElements = width * height;
        const unsigned int elementsPerThread = (height / maxThreads) * width;

        for (int i = 0; i < maxThreads; ++i) {
            unsigned int begin = i * elementsPerThread;
            unsigned int end = (i == maxThreads - 1) ? totalElements : (i + 1) * elementsPerThread;

            threadPool.emplace_back(([width, height, func](int begin, int end, float* a, float* b, float* out) {
                for (int i = begin; i < end; ++i) {
                    auto xf = static_cast<double>(i % width);
                    auto yf =  static_cast<double>(i / width);
                    out[i] = static_cast<float>(func(a[i], b[i], xf, yf, width, height));
                }

            }), begin, end, a, b, out);
        }

        for (auto &t : threadPool) {
            t.join();
        }

        threadPool.clear();
    }

    void MultithreadedNoiseExecutor::createPixelBufferFromData(unsigned int* out, float *in, int width, int height, ImageOp &func) {
        const unsigned int totalElements = width * height;
        const unsigned int elementsPerThread = (height / maxThreads) * width;

        for (int i = 0; i < maxThreads; ++i) {
            unsigned int begin = i * elementsPerThread;
            unsigned int end = (i == maxThreads - 1) ? totalElements : (i + 1) * elementsPerThread;

            threadPool.emplace_back(([width, height, func](int begin, int end, float* buffer, unsigned int* out) {
                for (int i = begin; i < end; ++i) {
                    auto xf = static_cast<double>(i % width);
                    auto yf =  static_cast<double>(i / width);
                    out[i] = func(buffer[i], xf, yf, width, height);
                }
            }), begin, end, in, out);
        }

        for (auto &t : threadPool) {
            t.join();
        }

        threadPool.clear();
    }

    void MultithreadedNoiseExecutor::combine(float *a, float *b, float *c, float *out, int width, int height, const Combine3NoiseOp &func) {

        const unsigned int totalElements = width * height;
        const unsigned int elementsPerThread = (height / maxThreads) * width;

        for (int i = 0; i < maxThreads; ++i) {
            unsigned int begin = i * elementsPerThread;
            unsigned int end = (i == maxThreads - 1) ? totalElements : (i + 1) * elementsPerThread;

            threadPool.emplace_back(([width, height, func](int begin, int end, float* a, float* b, float* c, float* out) {
                for (int i = begin; i < end; ++i) {
                    auto xf = static_cast<double>(i % width);
                    auto yf =  static_cast<double>(i / width);
                    out[i] = static_cast<float>(func(a[i], b[i], c[i], xf, yf, width, height));
                }

            }), begin, end, a, b, c, out);
        }

        for (auto &t : threadPool) {
            t.join();
        }

        threadPool.clear();
    }


}

