#include "OctaveNoise.h"

OctaveNoise::OctaveNoise() {

}

OctaveNoise::OctaveNoise(unsigned int seed, int var2) {
    this->octaves = var2;

    for (int var3 = 0; var3 < var2; ++var3) {
        this->perlin.push_back(new PerlinNoise(seed));
    }
}
OctaveNoise::~OctaveNoise() {
    for (auto& n : perlin) {
        delete n;
    }
    perlin.clear();
}

double OctaveNoise::compute(double var1, double var3) {
    double var5 = 0.0;
    double var7 = 1.0;

    for (int var9 = 0; var9 < this->octaves; ++var9) {
        var5 += this->perlin[var9]->compute(var1 / var7, var3 / var7) * var7;
        var7 *= 2.0;
    }

    return var5;
}