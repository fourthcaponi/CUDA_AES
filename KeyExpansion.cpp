// Filename: KeyExpansion.cpp
// SIUE CS490-005
// Spring 2017
// Authors: David Shimkus and Michael Caponi
//
// NOTE: Byte operations are performed on char arrays.
//
// Revisions:
// 04/26/2017 | DS | Creation.

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>

#include "State.h"
#include "Matrices.h"

using namespace std;

void RotWord(Word &word)
{
    unsigned char tempBytes[4];
    for(int i = 0; i < 4; i++)
    {
        //place the appropriate left shifted bytes into the temp variable
        tempBytes[i] = word.bytes[(i+1)%4];
    }
    for(int k = 0; k < 4; k++)
    {
        //place the temp var's data into the original input word
        word.bytes[k] = tempBytes[k];
    }
}

void SubWord(Word &word)
{
    for(int i = 0; i < 4; i++)
    {
        //get the ASCII value as well as its decimal value
        int byteTemp = word.bytes[i];

        //get the leftmost 4 bits aka the COLUMN
        byteTemp = (byteTemp >> 4) & ((1 << 4)-1); //leftmost 4 bits
        int column = byteTemp;

        //get the rightmost 4 bits aka the ROW
        byteTemp = word.bytes[i];
        byteTemp = (byteTemp >> 0) & ((1 << 4) - 1); //rightmost 4 bits
        int row = byteTemp;

        //set the original bytes on the passed in matrix to the new bytes
        word.bytes[i] = Matrix_Rijndael[column][row];
    }
}