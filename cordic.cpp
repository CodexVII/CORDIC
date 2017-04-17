/*
 * cordic.cpp
 *
 *  Created on: 13 Apr 2017
 *      Author: keita
 */
#include "cordic.h"

// Represents 1/k where k = 1.64676 in decimal
#define CORDIC_SCALER 0x09b74	// 0.607253 in decimal

// Pre-generated 2.16 fixed-point integers of atan(2^-1)
int atan_table[] = { 0x0c90f, 0x076b1, 0x03eb6, 0x01fd5, 0x00ffa, 0x007ff,
		0x003ff, 0x001ff, 0x000ff, 0x0007f, 0x0003f, 0x0001f, 0x0000f, 0x00007,
		0x00003, 0x00001, 0x00000, 0x00000 };

using namespace std;

int main() {
	runTest(0);
	runTest(M_PI/2);
	runTest(M_PI/4);
	runTest(-M_PI/2);
	runTest(-M_PI/4);

	cout << "==============================================" << endl;
	cout << "<< Accuracy Test >> " << endl;
	cout << "==============================================" << endl;
	double sin_diff[100] = {0};
	double cos_diff[100] = {0};
	int increment = 0;
	for (int i = -50; i < 50; i++) {
		// prepare inputs for CORDIC.
		float a = (M_PI/2) * (i / 50.0);
		cout << "=================" << endl;
		cout << "<< ITERATION " << increment << " >>" << endl;
		cout << "=================" << endl;
		double iter_sin_diff = 0;
		double iter_cos_diff = 0;
		runTest(a, &iter_sin_diff, &iter_cos_diff);
		sin_diff[i] = iter_sin_diff;
		cos_diff[i] = iter_cos_diff;

		increment ++;
	}
	cout << "==============================================" << endl;
	cout << "FINAL DIFFERENCE" << endl;
	cout << "==============================================" << endl;
	cout << "Cosine average difference: " << arrayAverage(cos_diff, 100)<< endl;
	cout << "Sine average difference: " << arrayAverage(sin_diff, 100)<< endl;
	cout << "Cosine average difference: " << bitset<BIT_SIZE> (FLOAT_TO_FIXED(arrayAverage(cos_diff, 100)))<< endl;
	cout << "Sine average difference: " << bitset<BIT_SIZE> (FLOAT_TO_FIXED(arrayAverage(sin_diff, 100)))<< endl;
}

double arrayAverage(double array[], int size){
	double result = 0;
	for(int i=0; i<size; i++){
		result += array[i];
	}
	return result /=size;
}

void runTest(double angle){
	int fixed = FLOAT_TO_FIXED(angle);
	int sine, cosine;
	cordic(fixed, &sine, &cosine);

	double sin_diff = abs(sin(angle) - FIXED_TO_FLOAT(sine));
	double cosine_diff = abs(cos(angle) - FIXED_TO_FLOAT(cosine));
	cout << "==========================================" << endl;
	cout << "Test for angle: " << angle << endl;
	cout << "==========================================" << endl;
	cout << "--Decimal comparisons--" << endl;
	cout << "Actual Sine:   " << sin(angle) << endl;
	cout << "CORDIC Sine:   " << FIXED_TO_FLOAT(sine)<< endl;
	cout << "Actual Cosine: " << cos(angle) << endl;
	cout << "CORDIC Cosine: " << FIXED_TO_FLOAT(cosine) << endl;
	cout << "Sine Difference:   " << sin_diff << endl;
	cout << "Cosine Difference: " << cosine_diff << endl;
	cout << "--Binary comparisons--" << endl;
	cout << "Actual Sine:   " << bitset<BIT_SIZE> FLOAT_TO_FIXED(sin(angle)) << endl;
	cout << "CORDIC Sine:   " << bitset<BIT_SIZE> (sine) << endl;
	cout << "Actual Cosine: " << bitset<BIT_SIZE> FLOAT_TO_FIXED(cos(angle)) << endl;
	cout << "CORDIC Cosine: " << bitset<BIT_SIZE> (cosine) << endl;
	cout << "Sine DIFFERENCE:   " << bitset<BIT_SIZE> FLOAT_TO_FIXED(sin_diff) << endl;
	cout << "Cosine DIFFERENCE: " << bitset<BIT_SIZE> FLOAT_TO_FIXED(cosine_diff) << endl;
}


void runTest(double angle, double *sin_diff, double *cos_diff){
	int fixed = FLOAT_TO_FIXED(angle);
	int sine, cosine;
	cordic(fixed, &sine, &cosine);
	*sin_diff = abs(sin(angle) - FIXED_TO_FLOAT(sine));
	*cos_diff = abs(cos(angle) - FIXED_TO_FLOAT(cosine));

	cout << "==========================================" << endl;
	cout << "Test for angle: " << angle << endl;
	cout << "==========================================" << endl;
	cout << "--Decimal comparisons--" << endl;
	cout << "Actual Sine:   " << sin(angle) << endl;
	cout << "CORDIC Sine:   " << FIXED_TO_FLOAT(sine)<< endl;
	cout << "Actual Cosine: " << cos(angle) << endl;
	cout << "CORDIC Cosine: " << FIXED_TO_FLOAT(cosine) << endl;
	cout << "Sine Difference:   " << *sin_diff<< endl;
	cout << "Cosine Difference: " << *cos_diff << endl;
	cout << "--Binary comparisons--" << endl;
	cout << "Actual Sine:   " << bitset<BIT_SIZE> FLOAT_TO_FIXED(sin(angle)) << endl;
	cout << "CORDIC Sine:   " << bitset<BIT_SIZE> (sine) << endl;
	cout << "Actual Cosine: " << bitset<BIT_SIZE> FLOAT_TO_FIXED(cos(angle)) << endl;
	cout << "CORDIC Cosine: " << bitset<BIT_SIZE> (cosine) << endl;
	cout << "Sine DIFFERENCE:   " << bitset<BIT_SIZE> (FLOAT_TO_FIXED(*sin_diff)) << endl;
	cout << "Cosine DIFFERENCE: " << bitset<BIT_SIZE> (FLOAT_TO_FIXED(*cos_diff)) << endl;
}

/**
 * cordic
 *
 * Finds the sine and cosine representations of the supplied angle.
 * All operations are done using 2.16 fixed-point representation and
 * with a series of adds, subtracts and shifts.
 *
 * @param angle: 	final angle in 2.16 fixed-point form
 * @param sin: 		sine of requested angle in 2.16 fixed-point form
 * @parma cos:		cosine of requested angle in 2.16 fixed-point form
 */
void cordic(int angle, int *sine, int *cosine) {
//	cout << FIXED_TO_FLOAT(angle) << endl;
//	cout << bitset<BIT_SIZE>(angle) << endl;

	// set initial parameters to find sine and cosine values with CORDIC
	int curr_x = CORDIC_SCALER;
	int curr_y = 0;
	int curr_z = angle;

	// checks for repositioning of angle

	// begin CORDIC iterations
	int d;	// Checks sign of current angle.
	int new_x, new_y, new_z;		// used for new iteration values
	for (int i = 0; i < BIT_SIZE; i++) {
		d = curr_z >> (BIT_SIZE - 1);// right shift sign bit to LSB to check sign

		// assign new values to x and y components.
		// get new angle difference and assign to z
		// operation of assignment depends on the sign of the input angle.
		if (d == 0) {	// positive
			new_x = curr_x - (curr_y >> i);
			new_y = curr_y + (curr_x >> i);
			new_z = curr_z - atan_table[i];
		} else if (d == -1) {	// negative
			new_x = curr_x + (curr_y >> i);
			new_y = curr_y - (curr_x >> i);
			new_z = curr_z + atan_table[i];
		}

		// set current values to the next variables for the following iteration
		curr_x = new_x;
		curr_y = new_y;
		curr_z = new_z;

	}
	// iteration complete. extract sine and cosine values from CORDIC and return
	// to calling function. at the end of the iterations..
	// curr_x = cos(angle) and curr_y = sin(angle)
	*cosine = curr_x;
	*sine = curr_y;
}

