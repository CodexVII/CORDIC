/*
 * cordic_tables.cpp
 *
 * Dylan O' Connor Desmond - 13154117
 * Ian Lodovica            - 13131567
 */

#include "cordic.h"

using namespace std;

/**
 * generateTables
 *
 * Helper function which prints out the necessary hard-coded values for
 * the CORDIC algorithm. Converted to 2.16 fixed-point and then represented
 * in hex form.
 *
 * The calculated values include:
 * 	1. tan-1(2^-i) table used for lookup during the iteration phase
 * 	2. K and 1/K values for canceling out system gain
 */
void generateTables() {
	// Generate atan(2^-i) table
	cout << "-------ATAN TABLE--------" << endl;
	for (int i = 0; i < BIT_SIZE; i++) {
		// Calculate arctan(2^-i) and convert to 2.16 fixed-point
		int angle = FLOAT_TO_FIXED(atan(pow(2, -i)));

		// Print hex representation of 2.16 fixed-point values
		printHex(angle);
	}

	// Calculates the value of K which will be used to cancel out
	// the system gain inherent with the CORDIC processor.
	// Results to roughly 1.6
	float k = 1;
	for (int i = 0; i < BIT_SIZE; i++) {
		k *= sqrt(1 + pow(2, -2 * i));
	}
	cout << "------K--------" << endl;
	cout << "K: " << k << endl;
	cout << "1/K: " << 1 / k << endl;
	cout << "Fixed K: ";
	printHex(FLOAT_TO_FIXED(k));
	cout << "Fixed 1/k: ";
	printHex(FLOAT_TO_FIXED(1 / k));

}

/**
 * printHex
 *
 * Helper method which prints an integer in its hex representation with
 * some extra formatting for readability.
 *
 * @param hex: integer to be printed as hex
 */
void printHex(int hex) {
	cout << "0x" << std::setfill('0') << std::hex << std::setw(5) << hex
			<< endl;
}

