#ifndef PAV_ANALYSIS_H
#define PAV_ANALYSIS_H

int sgn(double x);
float compute_power(const float *x, unsigned int N);
float compute_am(const float *x, unsigned int N);
float compute_zcr(const float *x, unsigned int N, float fm);

#endif	/* PAV_ANALYSIS_H	*/
