// Filename: MultInverse.cpp
// SIUE CS490-005
// Spring 2017
// Authors: David Shimkus and Michael Caponi
//
// NOTE: Byte operations are performed on char arrays.
//
// Revisions:
// 04/15 | DS | Creation.
// 04/24 | DS | Changed to deal with the Matrices.h
// 04/25 | DS | I don't think this file will be used anymore.

#ifndef MULT_INV
#define MULT_INV


#include <iostream>

#include "Matrices.h"


using namespace std;
 
// A naive method to find modulor multiplicative inverse of
// 'a' under modulo 'm'
int modInverse(int a, int m);


int modInverse(int a, int m)
{
    a = a%m;
    for (int x=1; x<m; x++)
    {
         if ((a*x) % m == 1)
         {
            return x;
         }
          
    }
    return -1; 
}




#endif