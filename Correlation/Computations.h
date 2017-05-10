#ifndef _Computations_H_
#define _Computations_H_


float getCorrelation(char **data, int dataSize);
float getMean(float *IntList, int dataSize);
float finalCalculation(float *AB, float *AA, float *BB, int dataSize);
float largestSpread(char**data, int dataSize);

#endif
