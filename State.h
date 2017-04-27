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

#ifndef STATE_H
#define STATE_H

#include <iostream>
#include <iomanip>

using namespace std;

struct State
{
    unsigned char bytes[4][4];
    int ByteSub_values[4][4];

    void print()
    {
        cout << endl;
        cout << "{ { " << setfill('0') << setw(2) << hex << (int)bytes[0][0] << ", " << setfill('0') << setw(2) << hex << (int)bytes[0][1] << ", " << setfill('0') << setw(2) << hex << (int)bytes[0][2] << ", " << setfill('0') << setw(2) << hex << (int)bytes[0][3] << " },\n";
	    cout << "  { " << setfill('0') << setw(2) << hex << (int)bytes[1][0] << ", " << setfill('0') << setw(2) << hex << (int)bytes[1][1] << ", " << setfill('0') << setw(2) << hex << (int)bytes[1][2] << ", " << setfill('0') << setw(2) << hex << (int)bytes[1][3] << " },\n";
	    cout << "  { " << setfill('0') << setw(2) << hex << (int)bytes[2][0] << ", " << setfill('0') << setw(2) << hex << (int)bytes[2][1] << ", " << setfill('0') << setw(2) << hex << (int)bytes[2][2] << ", " << setfill('0') << setw(2) << hex << (int)bytes[2][3] << " },\n";
	    cout << "  { " << setfill('0') << setw(2) << hex << (int)bytes[3][0] << ", " << setfill('0') << setw(2) << hex << (int)bytes[3][1] << ", " << setfill('0') << setw(2) << hex << (int)bytes[3][2] << ", " << setfill('0') << setw(2) << hex << (int)bytes[3][3] << " } }\n\n";
    }


};

//for key expansion
struct Word
{
    unsigned char bytes[4];

    void print()
    {
        cout << endl;
        cout << "{ " << setfill('0') << setw(2) << hex << (int)bytes[0] << ", " << setfill('0') << setw(2) << hex << (int)bytes[1] << ", " << setfill('0') << setw(2) << hex << (int)bytes[2] << ", " << setfill('0') << setw(2) << hex << (int)bytes[3] << " }\n\n";
    }
};

#endif