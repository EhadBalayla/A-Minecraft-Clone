#include "MathHelper.h"

int MathHelper::floor_double(double var0) {
	int var2 = (int)var0;
	return var0 < (double)var2 ? var2 - 1 : var2;
}

float MathHelper::sin(float var0) {
	return SIN_TABLE[(int)(var0 * 10430.378F) & '\uffff'];
}

float MathHelper::cos(float var0) {
	return SIN_TABLE[(int)(var0 * 10430.378F + 16384.0F) & '\uffff'];
}