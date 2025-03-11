#ifndef MATH_OPS_H
#define MATH_OPS_H

#define PI 3.14159265359f

//#include "math.h"

float fmaxf(float x, float y);
float fminf(float x, float y);
float fmaxf3(float x, float y, float z);
float fminf3(float x, float y, float z);
int limit_norm(float *x, float *y, float limit);
void limit_abs(float *x, float limit);
void constrain(float *x, float min, float max);
void constrain_int(int *x, int min, int max);
void slew_rate_lim(float inp, float *out, float rate, float lim);

#endif
