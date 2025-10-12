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

    inline void setSeed(int64_t newSeed) {
        seed = (newSeed ^ multiplier) & mask;
    }

    int32_t next(int bits) {
        seed = (seed * multiplier + addend) & mask;
        return static_cast<int32_t>(seed >> (48 - bits));
    }

    inline  int32_t nextInt() {
        return next(32);
    }

    inline int32_t nextInt(int32_t bound) {
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

    inline  double nextDouble() {
        return (((int64_t)next(26) << 27) + next(27)) / static_cast<double>(1LL << 53);
    }

    inline float nextFloat() {
        return next(24) / (static_cast<float>(1 << 24));
    }

    inline bool nextBoolean() {
        return next(1) != 0;
    }
    inline int64_t nextLong() {
        int64_t high = static_cast<int64_t>(next(32)) << 32;
        int64_t low = static_cast<int64_t>(next(32)) & 0xFFFFFFFFLL;
        return high | low;
    }
};

