#ifndef HEADER_FILE
#define HEADER_FILE

#include <math.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

int randInRange(int max);
double rand_un();
double randn (double mu, double sigma);

int get_index_based_on_probability(double *, int);

#ifdef __cplusplus
}
#endif

#endif
