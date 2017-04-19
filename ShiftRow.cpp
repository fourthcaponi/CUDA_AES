// Filename: ShiftRow.cpp
// SIUE CS490-005
// Spring 2017
// Authors: David Shimkus and Michael Caponi
//
// NOTE: Byte operations are performed on char arrays.
//
// Revisions:
// 04/19/2017 | DS | Creation.

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
            temp.bytes[i][j] = 0x63; //hex
        }
    }


    return temp;
}