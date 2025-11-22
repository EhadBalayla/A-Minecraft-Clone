#include "CombinedNoise.h"

CombinedNoise::CombinedNoise() {

}

CombinedNoise::CombinedNoise(Noise* var1, Noise* var2) {
	noise1 = var1;
	noise2 = var2;
}
CombinedNoise::~CombinedNoise() {
	delete noise1;
	delete noise2;
}

double CombinedNoise::compute(double var1, double var3) {
	return this->noise1->compute(var1 + this->noise2->compute(var1, var3), var3);
}