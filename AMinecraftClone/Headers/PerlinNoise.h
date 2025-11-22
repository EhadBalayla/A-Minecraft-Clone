#pragma once
#include "Noise.h"

class PerlinNoise : public Noise{
public:
    PerlinNoise(unsigned int seed);
    double compute(double var1, double var3) override;

private:
    int noise[512];

    double fade(double t);
    double lerp(double a, double b, double t);
    double grad(int hash, double x, double y, double z);
};
