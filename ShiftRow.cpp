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

State ShiftRow(State input)
{
    State temp;
    
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            //grab the input bytes
            temp.bytes[i][j] = input.ByteSub_values[i][j];

            //perform the shift as dependent upon the row
            temp.bytes[i][j] = temp.bytes[i][j] << i;
        }
    }


    return temp;
}

State InvShiftRow(State input)
{
    State temp;
    
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            //grab the input bytes
            temp.bytes[i][j] = input.ByteSub_values[i][j];

            //perform the shift as dependent upon the row
            temp.bytes[i][j] = temp.bytes[i][j] >> i;
        }
    }


    return temp;
}