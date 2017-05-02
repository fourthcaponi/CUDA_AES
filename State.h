// Filename: State.h
// SIUE CS490-005
// Spring 2017
// Authors: David Shimkus and Michael Caponi
//
// NOTE: Byte operations are performed on char arrays.
//
// Revisions:
// 04/19/2017 | DS | Creation.
// 04/25/2017 | DS | Added the print() function.
// 04/26/2017 | DS | Added the Word struct for key expansion.
// 04/28/2017 | DS | Added a printAscii() function.
// 05/01/2017 | MC | Convert to CUDA.

#ifndef STATE_H
#define STATE_H

#include <iostream>
#include <iomanip>

using namespace std;

#define BLOCK_SIZE_BIT 128
#define BLOCK_SIZE_CHAR 16   //a single character is stored as a byte, so 16 bytes = 128 bits

//this struct for the array of blocks implementation
//TODO: capitalize this guy everywhere for my OCD
struct block
{
	unsigned char text[BLOCK_SIZE_CHAR]; //divide by 8 b/c 1 char = 1 byte
};

struct State
{
	int bytes[4][4];

	/*void print()
	{
		cout << endl;
		cout << "{ { " << setfill('0') << setw(2) << hex << (int)bytes[0][0] << ", " << setfill('0') << setw(2) << hex << (int)bytes[0][1] << ", " << setfill('0') << setw(2) << hex << (int)bytes[0][2] << ", " << setfill('0') << setw(2) << hex << (int)bytes[0][3] << " },\n";
		cout << "  { " << setfill('0') << setw(2) << hex << (int)bytes[1][0] << ", " << setfill('0') << setw(2) << hex << (int)bytes[1][1] << ", " << setfill('0') << setw(2) << hex << (int)bytes[1][2] << ", " << setfill('0') << setw(2) << hex << (int)bytes[1][3] << " },\n";
		cout << "  { " << setfill('0') << setw(2) << hex << (int)bytes[2][0] << ", " << setfill('0') << setw(2) << hex << (int)bytes[2][1] << ", " << setfill('0') << setw(2) << hex << (int)bytes[2][2] << ", " << setfill('0') << setw(2) << hex << (int)bytes[2][3] << " },\n";
		cout << "  { " << setfill('0') << setw(2) << hex << (int)bytes[3][0] << ", " << setfill('0') << setw(2) << hex << (int)bytes[3][1] << ", " << setfill('0') << setw(2) << hex << (int)bytes[3][2] << ", " << setfill('0') << setw(2) << hex << (int)bytes[3][3] << " } }\n\n";
	}

	void printAscii()
	{
		cout << endl;
		cout << "{ { " << (char)bytes[0][0] << ", " << (char)bytes[0][1] << ", " << (char)bytes[0][2] << ", " << (char)bytes[0][3] << " },\n";
		cout << "  { " << (char)bytes[1][0] << ", " << (char)bytes[1][1] << ", " << (char)bytes[1][2] << ", " << (char)bytes[1][3] << " },\n";
		cout << "  { " << (char)bytes[2][0] << ", " << (char)bytes[2][1] << ", " << (char)bytes[2][2] << ", " << (char)bytes[2][3] << " },\n";
		cout << "  { " << (char)bytes[3][0] << ", " << (char)bytes[3][1] << ", " << (char)bytes[3][2] << ", " << (char)bytes[3][3] << " } }\n\n";

	}*/
};

//for key expansion
struct Word
{
	int bytes[4];

	/*void print()
	{
		cout << endl;
		cout << "{ " << setfill('0') << setw(2) << hex << (int)bytes[0] << ", " << setfill('0') << setw(2) << hex << (int)bytes[1] << ", " << setfill('0') << setw(2) << hex << (int)bytes[2] << ", " << setfill('0') << setw(2) << hex << (int)bytes[3] << " }\n\n";
	}*/
};

#endif
