/*
 * cordic.h
 *
 * Dylan O' Connor Desmond - 13154117
 * Ian Lodovica            - 13131567
 */

#ifndef CORDIC_H_
#define CORDIC_H_

#include <iostream>
#include <math.h>
#include <iostream>
#include <iomanip>
#include <bitset>
#include <cmath>

// Basic 2.16 defines
#define BIT_SIZE 18
#define FRACTIONAL_BITS 16
#define FIXED_POINT_ONE (1 << FRACTIONAL_BITS)

// INT<->FIXED conversions
#define INT_TO_FIXED(x) ((x) << FRACTIONAL_BITS)
#define FIXED_TO_INT(x) ((x) >> FRACTIONAL_BITS)

// FLOAT<->FIXED conversions
#define FLOAT_TO_FIXED(x) ((int)((x) * FIXED_POINT_ONE))
#define FIXED_TO_FLOAT(x) (((float)(x)) / FIXED_POINT_ONE)

#define DEGREES_TO_RADIANS(x) ((x)*((M_PI)/(180)))
#define RADIANS_TO_DEGREES(x) ((x)*((180)/(M_PI)))

void printHex(int hex);
void cordic(int angle, int *sin, int *cos);
double arrayAverage(double array[], int size);
void runTest(double angle);
void runTest(double angle, double *sin_diff, double *cos_diff);
#endif /* CORDIC_H_ */
