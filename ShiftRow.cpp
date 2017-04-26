// Filename: ShiftRow.cpp
// SIUE CS490-005
// Spring 2017
// Authors: David Shimkus and Michael Caponi
//
// NOTE: Byte operations are performed on char arrays.
//
// Revisions:
// 04/19/2017 | DS | Creation.
// 04/24/2017 | DS | Added the shift operators.
// 04/25/2017 | DS | Added the &'s to parameters.  Changed functions to return void.

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <sys/stat.h> //for filesize
#include <vector>

using namespace std;

#include "ShiftRow.h"
#include "State.h"

void ShiftRow(State &input)
{
    for(int i = 0; i < 4; i++)
    {
        unsigned char tempBytes[4];
        for(int j = 0; j < 4; j++)
        {
            //perform the left shift as dependent upon the row
            tempBytes[j] = input.bytes[i][(j+i)%4];
        }
        for(int k = 0; k < 4; k++)
        {
            input.bytes[i][k] = tempBytes[k];
        }
    }
}

void InvShiftRow(State &input)
{  
    for(int i = 0; i < 4; i++)
    {
        unsigned char tempBytes[4];
        for(int j = 0; j < 4; j++)
        {
            //perform the right shift as dependent upon the row
            tempBytes[j] = input.bytes[i][(j-i)%4];
        }
        for(int k = 0; k < 4; k++)
        {
            input.bytes[i][k] = tempBytes[k];
        }
    }
}