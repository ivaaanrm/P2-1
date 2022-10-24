#include <math.h>
#include "pav_analysis.h"
#include <stdio.h>

float compute_power(const float *x, unsigned int N) {
    int i;
    double sum = 0.0;
    for(i = 0; i < N; i++){
        sum += x[i]*x[i];
    }
    return 10*log10(sum/N);
}

float compute_am(const float *x, unsigned int N) {
    int i;
    double sum = 0.0;
    for(i = 0; i < N; i++){
        sum += fabs(x[i]);
    }
    return sum/N;
}

float compute_zcr(const float *x, unsigned int N, float fm) {
    int i;
    double sum = 0.0;
    for (i = 0; i < N; i++){
        if (sgn(x[i]) != sgn(x[i-1])){
            sum++;
        }
    }
    return fm/(2*(N-1))*sum;
}

int sgn(double x) {
  if (x > 0.0) return 1;
  if (x < 0.0) return -1;
  return 0;
}
