// Filename: ByteSub.cpp
// SIUE CS490-005
// Spring 2017
// Authors: David Shimkus and Michael Caponi
//
// NOTE: Byte operations are performed on char arrays.
//
// Revisions:
// 04/19/2017 | DS | Creation.
// 04/24/2017 | DS | Added the hex/dec stuff, rips each byte into 4 bit chunks.
// 04/24/2017 | DS | Work on InvByteSub.  

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <sys/stat.h> //for filesize
#include <vector>

using namespace std;

#include "ByteSub.h"
#include "State.h"
#include "MultInverse.cpp"
#include "Matrices.h"

void ByteSub(State &input)
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            //get the ASCII value as well as its decimal value
            int byteTemp = input.bytes[i][j];
            //cout << "ASCII: " << input.bytes[i][j] << " " << endl;
            //cout << "Decimal: " << dec << (int)input.bytes[i][j] << " " << endl;
            //cout << "Hex: " << hex << byteTemp << endl;

            //get the leftmost 4 bits aka the COLUMN
            byteTemp = input.bytes[i][j];
            byteTemp = (byteTemp >> 4) & ((1 << 4)-1); //leftmost 4 bits
            //cout << "Leftmost 4 bits: " << hex << byteTemp << endl;
            int column = byteTemp;

            //get the rightmost 4 bits aka the ROW
            byteTemp = input.bytes[i][j];
            byteTemp = (byteTemp >> 0) & ((1 << 4) - 1); //rightmost 4 bits
            //cout << "Rightmost 4 bits: " << hex << byteTemp << endl;
            int row = byteTemp;

            //access the constant matrix to perform the byte sub
            //cout << "Matrix_ByteSub[" << dec << column << "][" << dec << row << "]: " << hex << Matrix_ByteSub[column][row] << endl;

            //set the original bytes on the passed in matrix to the new bytes
            //stringstream ss;
            //ss << "0x" << Matrix_ByteSub[column][row];
            //input.ByteSub_values[i][j] = stoi(ss.str());
            //input.ByteSub_values[i][j] = Matrix_ByteSub[column][row];
            //cout << "ByteSub_values[i][j]: " << input.ByteSub_values[i][j] << endl;
            input.bytes[i][j] = Matrix_ByteSub[column][row];

            //cout << endl << endl;
        }
    }

/*
    //create our output state object
    State output;

    //populate it with the appropriate values from the byte sub operation
    //TODO: don't return this object at all and have it perform in place in memory?
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            output.ByteSub_values[i][j] = input.ByteSub_values[i][j];
        }
    }

    return output;
    */
}

void InvByteSub(State &input)
{
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            //get the ASCII value as well as its decimal value
            //int byteTemp = input.ByteSub_values[i][j];
            
            // cout << "byteTemp: " << byteTemp << endl;

            cout << input.ByteSub_values[i][j];

            
            cout << endl << endl;


            //get the ASCII value as well as its decimal value
            int byteTemp = input.ByteSub_values[i][j];
            cout << "ASCII: " << input.ByteSub_values[i][j] << " " << endl;
            cout << "Decimal: " << dec << (int)input.ByteSub_values[i][j] << " " << endl;
            cout << "Hex: " << hex << byteTemp << endl;

            //get the leftmost 4 bits aka the COLUMN
            byteTemp = input.ByteSub_values[i][j];
            byteTemp = (byteTemp >> 4) & ((1 << 4)-1); //leftmost 4 bits
            cout << "Leftmost 4 bits: " << hex << byteTemp << endl;
            int column = byteTemp;

            //get the rightmost 4 bits aka the ROW
            byteTemp = input.ByteSub_values[i][j];
            byteTemp = (byteTemp >> 0) & ((1 << 4) - 1); //rightmost 4 bits
            cout << "Rightmost 4 bits: " << hex << byteTemp << endl;
            int row = byteTemp;

            //access the constant matrix to perform the byte sub
            cout << "Matrix_InvByteSub[" << dec << column << "][" << dec << row << "]: " << hex << Matrix_InvByteSub[column][row] << endl;

            //set the original bytes on the passed in matrix to the new bytes
            //stringstream ss;
            //ss << "0x" << Matrix_ByteSub[column][row];
            //input.ByteSub_values[i][j] = stoi(ss.str());
            input.ByteSub_values[i][j] = Matrix_InvByteSub[column][row];
            cout << "ByteSub_values[i][j]: " << input.ByteSub_values[i][j] << endl;


        }
    }   

    /*
    //create our output state object
    State output;

    //populate it with the appropriate values from the byte sub operation
    //TODO: don't return this object at all and have it perform in place in memory?
    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {
            output.ByteSub_values[i][j] = input.ByteSub_values[i][j];
        }
    }

    return output;
*/

}

