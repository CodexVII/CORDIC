/*
 * cordic_tables.cpp
 *
 * Dylan O' Connor Desmond - 13154117
 * Ian Lodovica            - 13131567
 */

#include "cordic.h"

using namespace std;

void generateTables() {
	// Generate atan(2^-i) table
	cout << "-------ATAN TABLE--------" << endl;
	for (int i = 0; i < BIT_SIZE; i++) {
		// Calculate arctan(2^-i) and convert to 2.16 fixed-point
		int angle = FLOAT_TO_FIXED(atan(pow(2, -i)));

		// Print hex representation of 2.16 fixed-point values
		printHex(angle);
	}

	// Calculate K
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

void printHex(int hex) {
	cout << "0x" << std::setfill('0') << std::hex << std::setw(5) << hex
			<< endl;
}

