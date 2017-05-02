// Filename: Cipher.cpp
// SIUE CS490-005
// Spring 2017
// Authors: David Shimkus and Michael Caponi
//
// This is where the magic happens.
//
// Revisions:
// 04/27/2017 | DS | Creation. Moved from Main.cpp for program flow.
// 04/28/2017 | DS | Functions now accept states instead of blocks for input.
// 04/29/2017 | DS | Changed the way the keyWords was getting passed in.
// 05/01/2017 | MC | Convert to CUDA.

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>

using namespace std;

#include "ByteSub.h"
#include "KeyAdd.h"
#include "MixColumn.h"
#include "ShiftRow.h"
#include "State.h"
#include "Matrices.h"

#include "cuda_runtime.h"
#include "device_launch_parameters.h"

//depending on the passed in round index will determine 
//which indeces of keyWords we will use

//note this "roundIndex WILL go all the way up to numRounds"
__global__ void Cipher(State &input, Word(&keyWords)[60], size_t size, int roundIndex, int numRounds)
{
	if (roundIndex == 0)
	{
		//pre-round transformation
		KeyAdd(input, keyWords, size, 0);
	}
	else if (roundIndex == numRounds)
	{
		ByteSub(input);
		ShiftRow(input);
		KeyAdd(input, keyWords, size, roundIndex);
	}
	else
	{
		ByteSub(input);
		ShiftRow(input);
		MixColumn(input);
		KeyAdd(input, keyWords, size, roundIndex);
	}

	cout << "---- Cipher Output Round #" << dec << roundIndex << " ----\n";
	input.print();

	cout << "---- Key for Round #" << dec << roundIndex << " ----\n";
	for(int i = roundIndex * 4; i < roundIndex*4+4; i ++)
	{
		keyWords[i].print();
	}

}

//note the roundIndex is BACKWARDS from how Dr. Gamage described it in class (?)
__global__ void Decrypt(State &input, Word(&keyWords)[60], size_t size, int roundIndex, int numRounds)
{
	if (roundIndex == 0)
	{
		KeyAdd(input, keyWords, size, numRounds);
		InvShiftRow(input);
		InvByteSub(input);
	}
	else if (roundIndex == numRounds)
	{
		//pre-round transformation
		KeyAdd(input, keyWords, size, 0);
	}
	else
	{
		KeyAdd(input, keyWords, size, numRounds - roundIndex);
		InvMixColumn(input);
		InvShiftRow(input);
		InvByteSub(input);
	}
}

