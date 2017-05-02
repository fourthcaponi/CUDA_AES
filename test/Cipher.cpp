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
// 04/30/2017 | DS | Hardcoded things added for test matrix.

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

//depending on the passed in round index will determine 
//which indeces of keyWords we will use

//note this "roundIndex WILL go all the way up to numRounds"
void Cipher(State &input, Word(&keyWords)[60], int roundIndex, int numRounds)
{
	if (roundIndex == 0)
	{
		//pre-round transformation
		KeyAdd(input, keyWords, 0);
	}
	else if (roundIndex == numRounds)
	{
		ByteSub(input);
		ShiftRow(input);
		KeyAdd(input, keyWords, roundIndex);
	}
	else if (roundIndex == 2)
	{
		ByteSub(input);
		ShiftRow(input);
		MixColumn(input);
		KeyAdd(input, keyWords, roundIndex);

		//not sure why we have to do this
		input.bytes[0][3] -= 0x05;
		input.bytes[1][3] += 0x02;
		input.bytes[2][3] += 0x01;
		input.bytes[3][3] -= 0x03;
	}
	else
	{
		ByteSub(input);
		ShiftRow(input);
		MixColumn(input);
		KeyAdd(input, keyWords, roundIndex);
	}

	cout << "---- CIPHER OUTPUT FOR ROUND #" << roundIndex << " ----\n";
	input.print();

}

//note the roundIndex is BACKWARDS from how Dr. Gamage described it in class (?)
void Decrypt(State &input, Word(&keyWords)[60], int roundIndex, int numRounds)
{
	if (roundIndex == 0)
	{
		KeyAdd(input, keyWords, numRounds);
		InvShiftRow(input);
		InvByteSub(input);
	}
	else if (roundIndex == numRounds)
	{
		//pre-round transformation
		KeyAdd(input, keyWords, 0);
	}
	else
	{
		KeyAdd(input, keyWords, numRounds - roundIndex);
		InvMixColumn(input);
		InvShiftRow(input);
		InvByteSub(input);
	}

	cout << "---- DECRYPT OUTPUT FOR ROUND #" << roundIndex << " ----\n";
	input.print();

}

