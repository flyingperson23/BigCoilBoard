
#include "math_ops.h"


float fmaxf(float x, float y){
    return (((x)>(y))?(x):(y));
    }

float fminf(float x, float y){
    return (((x)<(y))?(x):(y));
    }

float fmaxf3(float x, float y, float z){
    return (x > y ? (x > z ? x : z) : (y > z ? y : z));
    }

float fminf3(float x, float y, float z){
    return (x < y ? (x < z ? x : z) : (y < z ? y : z));
    }
    
int limit_norm(float *x, float *y, float limit){
    float norm = sqrt(*x * *x + *y * *y);
    //float norm;  // use the CORDICCCCCCC!!!
    //CordicModulus(*x, *y, &norm);
    if(norm > limit){
        *x = *x * limit/norm;
        *y = *y * limit/norm;
        return 1;  // return true if saturated
        }
    else { return 0; }
    return 0;
    }
    
void limit_abs(float *x, float limit){
    if (limit < 0) {limit = 0;}
    if(*x > limit){
        *x = limit;
        }
    if(*x < -limit){
        *x = -limit;
        }
    }


void constrain(float *x, float min, float max){
    if (*x > max) { *x = max; }
    if (*x < min) { *x = min; }
}

void constrain_int(int *x, int min, int max){
    if (*x > max) { *x = max; }
    if (*x < min) { *x = min; }
}

void slew_rate_lim(float inp, float *out, float rate, float lim){
    // implement a slew rate and implement a limit such that it cannot wind up if weird values are given.
    constrain(&inp, -lim, lim);
    if (inp > (rate + *out)) { *out += rate; }
    else if (inp < (-rate + *out)) { *out -= rate; }
    else { *out = inp; }
}
