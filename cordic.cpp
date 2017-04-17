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
	cout << "1/k" << bitset<BIT_SIZE> (CORDIC_SCALER) << endl;
	cout << "value" << FIXED_POINT_ONE << endl;
	int test_angle = FLOAT_TO_FIXED(-0.5);
	int test_sin, test_cos;
	cordic(test_angle, &test_sin, &test_cos);
	cout << "-0.5: " << bitset<BIT_SIZE> (test_angle) << endl;
	cout << "Test Sine: " << bitset<BIT_SIZE> (test_sin) << endl;
	cout << "Test Cosine: " << bitset<BIT_SIZE> (test_cos) << endl;
	for (int i=0; i<BIT_SIZE; i++){
		cout << bitset<BIT_SIZE> (atan_table[i]) << endl;
	}

	double sin_diff[10] = {0};
	double cos_diff[10] = {0};
	int increment = 0;
	for (float i = -5; i < 5; i++) {
		// prepare inputs for CORDIC.
//		float a = (M_PI/2) * (i / 5);
		float a = 0;
		cout << "==============================================" << endl;
		cout << "<< ITERATION " << i <<  ", ANGLE:" << a << " >>" << endl;
		cout << "==============================================" << endl;
		int angle = FLOAT_TO_FIXED(a);	// 2.16 fixed-point of pi/4
		int sine, cosine;	// prepared as outputs of cordic

		// run the cordic processor
		cordic(angle, &sine, &cosine);
		cout << bitset<BIT_SIZE> FLOAT_TO_FIXED(a) << endl;

		int a2 = ~angle;
		cout << bitset<BIT_SIZE>(a2) <<  endl;

		int a3 =a2+1;
		cout << bitset<BIT_SIZE>(a3) << endl;

		float a4 = FIXED_TO_FLOAT(a3);
		cout << a4 << endl;
		sin_diff[increment] = fabs(FIXED_TO_FLOAT(sine)-sin(a));
		cos_diff[increment] = fabs(FIXED_TO_FLOAT(cosine)-cos(a));
		// compare decimal values with actual
		cout << "-----decimal comparison-----" << endl;
		cout << "CORDIC sine: " << FIXED_TO_FLOAT(sine) << endl;
		cout << "Actual sine: " << sin(a) << endl;

		cout << "CORDIC cosine: " << FIXED_TO_FLOAT(cosine) << endl;
		cout << "Actual cosine: " << cos(a) << endl;

		// compare bit values with actual
		cout << "------bit comparison---------" << endl;
		cout << "CORDIC sine: " << bitset<BIT_SIZE>(sine) << endl;
		cout << "Actual sine: " << bitset<BIT_SIZE>(FLOAT_TO_FIXED(sin(a)))
				<< endl;
		cout << "SINE DIFFERENCE: " << bitset<BIT_SIZE> (abs(sine - FLOAT_TO_FIXED(sin(a)))) << endl;

		cout << "CORDIC cosine: " << bitset<BIT_SIZE>(cosine) << endl;
		cout << "Actual cosine: " << bitset<BIT_SIZE>(FLOAT_TO_FIXED(cos(a)))
				<< endl;
		cout << "COSINE DIFFERENCE: " << bitset<BIT_SIZE> (abs(sine - FLOAT_TO_FIXED(sin(a)))) << endl;


		// compare bit values with actual
		cout << "------hex comparison---------" << endl;
		cout << "Sine: ";
		printHex(sine);
		cout << "Cosine: ";
		printHex(cosine);

		increment ++;
	}
	cout << "==============================================" << endl;
	cout << "FINAL DIFFERENCE" << endl;
	cout << "==============================================" << endl;
	cout << "Cosine average difference: " << arrayAverage(cos_diff, 20)<< endl;
		cout << "Sine average difference: " << arrayAverage(sin_diff, 20)<< endl;
	cout << "Cosine average difference: " << bitset<BIT_SIZE> (FLOAT_TO_FIXED(arrayAverage(cos_diff, 20)))<< endl;
	cout << "Sine average difference: " << bitset<BIT_SIZE> (FLOAT_TO_FIXED(arrayAverage(sin_diff, 20)))<< endl;
}

double arrayAverage(double array[], int size){
	double result = 0;
	for(int i=0; i<size; i++){
		result += array[i];
	}
	return result /=size;
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
	cout << FIXED_TO_FLOAT(angle) << endl;
	cout << bitset<BIT_SIZE>(angle) << endl;

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

		//debugging
		cout << "Current X (cosine): " << bitset<BIT_SIZE>(curr_x) << endl;
		cout << "Current Y (sine): " << bitset<BIT_SIZE>(curr_y) << endl;

	}
	// iteration complete. extract sine and cosine values from CORDIC and return
	// to calling function. at the end of the iterations..
	// curr_x = cos(angle) and curr_y = sin(angle)
	*cosine = curr_x;
	*sine = curr_y;
}

