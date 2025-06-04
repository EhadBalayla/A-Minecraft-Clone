#pragma once
#include <iostream>
#include <cstdint>
#include <climits>
#include <chrono>

class Random {
private:
    int64_t seed;

    static constexpr int64_t multiplier = 0x5DEECE66DLL;
    static constexpr int64_t addend = 0xBLL;
    static constexpr int64_t mask = (1LL << 48) - 1;

public:

    Random(int64_t seed) {
        setSeed(seed);
    }

    Random() : Random(std::chrono::time_point_cast<std::chrono::nanoseconds>(std::chrono::high_resolution_clock::now()).time_since_epoch().count()) {}

    void setSeed(int64_t newSeed) {
        seed = (newSeed ^ multiplier) & mask;
    }

    int32_t next(int bits) {
        seed = (seed * multiplier + addend) & mask;
        return static_cast<int32_t>(seed >> (48 - bits));
    }

    int32_t nextInt() {
        return next(32);
    }

    int32_t nextInt(int32_t bound) {
        if ((bound & -bound) == bound) { // Power of two
            return (int32_t)((bound * (int64_t)next(31)) >> 31);
        }

        int32_t bits, val;
        do {
            bits = next(31);
            val = bits % bound;
        } while (bits - val + (bound - 1) < 0);
        return val;
    }

    double nextDouble() {
        return (((int64_t)next(26) << 27) + next(27)) / (double)(1LL << 53);
    }

    float nextFloat() {
        return next(24) / ((float)(1 << 24));
    }

    bool nextBoolean() {
        return next(1) != 0;
    }
};

