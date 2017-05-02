// Filename: KeyAdd.cpp
// SIUE CS490-005
// Spring 2017
// Authors: David Shimkus and Michael Caponi
//
// NOTE: Byte operations are performed on char arrays.
//
// Revisions:
// 04/19/2017 | DS | Creation.
// 04/26/2017 | DS | Work on KeyAdd
// 04/27/2017 | DS | Updated to reflect Cipher() 
// 04/28/2017 | DS | Modified to reflect latest Cipher()
// 05/01/2017 | DS | Code cleanup.
// 05/01/2017 | DS | Removed size from parameter list.

#include <iostream>
#include <iomanip>

using namespace std;

#include "State.h"

//note KeyAdd is the inverse of itself
void KeyAdd(State &input, Word(&keyWords)[60], int round)
{
	int counter = round * 4; //appropriate offset

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			//perform the XOR operation
			//note the order of [j] then [i]			
			input.bytes[j][i] ^= keyWords[counter].bytes[j];
		}
		counter++;
	}
}