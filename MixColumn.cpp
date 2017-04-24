// Filename: MixColumn.cpp
// SIUE CS490-005
// Spring 2017
// Authors: David Shimkus and Michael Caponi
//
// NOTE: Byte operations are performed on char arrays.
//
// Revisions:
// 04/19/2017 | DS | Creation.
// 04/20/2017 | MC | Began implementation
// 04/24/2017 | MC | The functions are working correctly.

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <sys/stat.h> //for filesize
#include <vector>

using namespace std;

#include "ByteSub.h"
#include "MixColumn.h"

unsigned char gmul(unsigned char a, unsigned char b) {

	//Author: Sam Trenholme

	//GF(2^8) multiplication/addition
	unsigned char p = 0; // the product of the multiplication
	unsigned char counter;
	unsigned char hi_bit_set;
	for(counter = 0; counter < 8; counter++) {
		// if b is odd, then add (XOR) the corresponding a to p (final product = sum of all a's corresponding to odd b's)
		if((b & 1) == 1)
			p ^= a;
		// GF modulo: if a >= 128, then it will overflow when shifted left, so reduce
		hi_bit_set = (a & 0x80);
		a <<= 1;
		// if a's hi bit had a value of one prior to this rotation, exclusive or a with the hexadecimal number 0x1b
		// 0x1b = x^4 + x^3 + x + 1 of GF(2^8) irreducible polynomial
		if(hi_bit_set == 0x80) 
			a ^= 0x1b;
		// rotate b one bit to the right, discarding the low bit, and making the high (eighth from left) bit have a value of zero		
		b >>= 1;
	}
	return p;
}

void MixColumn(State input)
{
    cout << endl;

	unsigned char col[4];

    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
			col[j] = input.bytes[j][i];
		}

        /* apply the mixColumn on one column */
        mc_operation(col);

        /* put the values back into the state */
        for (int j = 0; j < 4; j++)
        {
            input.bytes[j][i] = col[j];
        }
    }

	cout << endl;

    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            int byteTemp = input.bytes[j][i];
            cout << "ASCII: " << input.bytes[j][i] << " ";
            cout << "Full byteTemp: " << byteTemp << " ";

            stringstream ss1;
            byteTemp = (byteTemp >> 0) & ((1 << 4)-1); //rightmost 4 bits
            cout << "Rightmost 4 bits: " << hex << byteTemp << endl;

            ss1 << hex << byteTemp;
		}
	}
}

void mc_operation(unsigned char *col) {

    unsigned char copyCol[4];
	for(int i = 0; i < 4; i++)
    {
		copyCol[i] = col[i];
    }

	col[0] = gmul(copyCol[0],2) ^ gmul(copyCol[1],3) ^ gmul(copyCol[2],1) ^ gmul(copyCol[3],1) ;

	col[1] = gmul(copyCol[0],1) ^ gmul(copyCol[1],2) ^ gmul(copyCol[2],3) ^ gmul(copyCol[3],1);

	col[2] = gmul(copyCol[0],1) ^ gmul(copyCol[1],1) ^ gmul(copyCol[2],2) ^ gmul(copyCol[3],3);

	col[3] = gmul(copyCol[0],3) ^ gmul(copyCol[1],1) ^ gmul(copyCol[2],1) ^ gmul(copyCol[3],2);
}