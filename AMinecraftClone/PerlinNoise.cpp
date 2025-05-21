#include "PerlinNoise.h"
#include <cmath>
#include <random>

PerlinNoise::PerlinNoise(unsigned int seed) {
    
    std::mt19937 rng(seed);
    
    int var2;
    for (var2 = 0; var2 < 256; this->noise[var2] = var2++) {
        
    }

    for (var2 = 0; var2 < 256; ++var2) {
        std::uniform_int_distribution<unsigned int> dist(var2, 255);
        int var3 = dist(rng);
        int var4 = this->noise[var2];
        this->noise[var2] = this->noise[var3];
        this->noise[var3] = var4;
        this->noise[var2 + 256] = this->noise[var2];
    }
}

double PerlinNoise::compute(double var1, double var3) {
    double var5 = 0.0;
    double var7 = var3;
    double var9 = var1;

    int var18 = (int)floor(var1) & 255;
    int var2 = (int)floor(var3) & 255;
    int var19 = (int)floor(0.0) & 255;

    var9 -= floor(var9);
    var7 -= floor(var7);
    var5 = 0.0 - floor(0.0);

    double var11 = fade(var9);
    double var13 = fade(var7);
    double var15 = fade(var5);

    int var4 = this->noise[var18] + var2;
    int var17 = this->noise[var4] + var19;
    var4 = this->noise[var4 + 1] + var19;
    var18 = this->noise[var18 + 1] + var2;
    var2 = this->noise[var18] + var19;
    var18 = this->noise[var18 + 1] + var19;
    return lerp(var15, lerp(var13, lerp(var11, grad(this->noise[var17], var9, var7, var5), grad(this->noise[var2], var9 - 1.0, var7, var5)), lerp(var11, grad(this->noise[var4], var9, var7 - 1.0, var5), grad(this->noise[var18], var9 - 1.0, var7 - 1.0, var5))), lerp(var13, lerp(var11, grad(this->noise[var17 + 1], var9, var7, var5 - 1.0), grad(this->noise[var2 + 1], var9 - 1.0, var7, var5 - 1.0)), lerp(var11, grad(this->noise[var4 + 1], var9, var7 - 1.0, var5 - 1.0), grad(this->noise[var18 + 1], var9 - 1.0, var7 - 1.0, var5 - 1.0))));
}


double PerlinNoise::fade(double t) {
	return t * t * t * (t * (t * 6.0 - 15.0) + 10.0);
}

double PerlinNoise::lerp(double a, double b, double t) {
	return b + a * (t - b);
}

double PerlinNoise::grad(int hash, double x, double y, double z) {
	double var7 = (hash &= 15) < 8 ? x : y;
	double var9 = hash < 4 ? y : (hash != 12 && hash != 14 ? z : x);
	return ((hash & 1) == 0 ? var7 : -var7) + ((hash & 2) == 0 ? var9 : -var9);
}