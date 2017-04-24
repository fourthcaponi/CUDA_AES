// Filename: ByteSub.cpp
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

#include "ByteSub.h"
#include "State.h"
#include "MultInverse.cpp"
#include "Matrices.h"

void ByteSub(State input)
{
    
    //int temp;
    
    cout << endl;

    for(int i = 0; i < 4; i++)
    {
        for(int j = 0; j < 4; j++)
        {


            /*
            temp = modInverse(input.bytes[i][j],256); 

            cout << "Input Byte ASCII:  " << input.bytes[i][j] << endl;
            int temp2 = input.bytes[i][j];
            cout << "Input Byte Decimal:" << temp2 << endl;
            cout << "modInverse: " << temp << endl;
            cout << endl << endl;
            */

            int byteTemp = input.bytes[i][j];
            cout << "ASCII: " << input.bytes[i][j] << " ";
            cout << "Full byteTemp: " << byteTemp << " ";

            stringstream ss1;
            byteTemp = (byteTemp >> 0) & ((1 << 4)-1); //rightmost 4 bits
            cout << "Rightmost 4 bits: " << hex << byteTemp << endl;

            ss1 << hex << byteTemp;

            //cout << endl << (ss1.str());

            //string::size_type sz;
            int i_dec = stoi(ss1.str(),nullptr,16);

            cout << "i_dec: " <<  i_dec << endl;

            /*
            byteTemp = temp;
            byteTemp = (byteTemp >> 4) & ((1 << 4)-1); //leftmost 4 bits
            cout << "Upper 4 bits: " << hex << byteTemp << endl;
            */



        }
    }

    

}