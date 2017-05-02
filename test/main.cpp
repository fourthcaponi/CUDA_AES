// Filename: main.cpp
// SIUE CS490-005
// Spring 2017
// Authors: David Shimkus and Michael Caponi
//
// NOTE: Byte operations are performed on char arrays.
//
// Revisions:
// 04/10/2017 | DS | Creation.  I/O and chopping message into chunks set up.
// 04/12/2017 | DS | Worked on the 'chunks' portion more.  BLOCK_SIZE char vs bit fixed.
// 04/24/2017 | DS | Plugged in ByteSub and InvByteSub things.
// 04/25/2017 | DS | Added the new print() function after each state.  Using test matrix.
// 04/26/2017 | DS | Added in the key input, numRounds stuff.  Code cleanup.
// 04/27/2017 | DS | Work on the key expansion.  KeyWords[0] to KeyWords[groupsize] working.
// 04/27/2017 | DS | Adding appropriate keyWords to keys (array of states).
// 04/27/2017 | DS | Removed array of key states - only going to use keyWords now.  Key Addition worked on.
// 04/28/2017 | DS | Placed much of this file into Cipher.cpp.  Code cleanup.
// 04/28/2017 | DS | Created MAX_MSG_SIZE and MAX_STATES for the array allocation issue.
// 04/28/2017 | DS | Fixed the array pass by reference issue.  New version of MixColumn.
// 04/29/2017 | DS | Put Cipher and Decrypt into a loop.  More debugging.
// 05/01/2017 | DS | Changed cipher output text file to ascii representation of the hex values.  
// 05/01/2017 | DS | Ripped out a bunch of unnecessary stuff and made this test file.

#include <iostream>			//for grabbing user input
#include <fstream>			//file i/o
#include <sstream>			//used for string manipulation
#include <string>			//for grabbing user input
#include <cstring>			//converts strings to 'c style' strings
#include <sys/stat.h> 		//for filesize	

//the external files
#include "ByteSub.h"		//for ByteSub() and InvByteSub()
#include "ShiftRow.h"		//for ShiftRow() and InvShiftRow()
#include "MixColumn.h"		//for MixColumn() and InvMixColumn()
#include "KeyAdd.h"			//for KeyAdd()
#include "State.h"			//for struct Block, State, Word
#include "Matrices.h" 		//for test matrices as well as S-boxes
#include "KeyExpansion.h"	//for RotWord and SubWord
#include "Cipher.h"

using namespace std;

#define BLOCK_SIZE_BIT 128
#define BLOCK_SIZE_CHAR 16

//max values for the array sizes
//note these could technically be shrunk for this test
#define MAX_MSG_CHAR 3000
#define MAX_STATES 200
#define MAX_KEYWORDS 60 

//TODO: accept keysize, filenames as arguments
int main()
{
	string messageName, keyName;
	int keySize = -1; //126, 192, or 256
	int numRounds = -1; //dependent on the keysize
	int numKeyWords = -1; //for the key expansion
	int groupSize = -1; //the number of 'words' in each group of key expansions

	printf("\nAES TEST\n");
	printf("\nBy David Shimkus and Michael Caponi\n");
	printf("Based on input from page 217 of the textbook\n");

	int temp;
	cout << "\nBegin? (y/n) ";
	cin >> temp;
	cout << "\n";

	//assume AES128
	keySize = 128;
	numRounds = 10;
	numKeyWords = 44;
	groupSize = 4;

	//dummy data as per pg 210 from the textbook
	int keyChars[16];
	for(int i = 0; i < 16; i++)
	{
		keyChars[i] = Matrix_TestKey[i];
	}

	//declare our array of keys that will be populated with the key expansions
	Word keyWords[MAX_KEYWORDS]; //the total "words" to be populated

	//begin key expansion
	//perform the initial population for keyWords[0] to keyWords[groupSize] 
	int offset = 0;
	for (int i = 0; i < groupSize; i++)
	{
		for (int j = offset; j < offset + 4; j++)
		{
			keyWords[i].bytes[j % 4] = keyChars[j];
		}
		offset += 4;
	}

	//compute the rest of the keyWords
	//TODO: modularize this (use groupsize and %'s) (maybe a function) !!
	if (keySize == 128)
	{
		Word tempWord;
		for (int i = 4; i < numKeyWords; i++)
		{
			if (i % 4 != 0)
			{
				for (int j = 0; j < 4; j++)
				{
					keyWords[i].bytes[j] = keyWords[i - 1].bytes[j] ^ keyWords[i - 4].bytes[j];
				}
			}
			else
			{
				//function g
				for (int k = 0; k < 4; k++)
				{
					tempWord.bytes[k] = keyWords[i - 1].bytes[k]; //initially populate the tempword		
				}

				//perform the rotation
				RotWord(tempWord);

				//apply the subword
				SubWord(tempWord);

				//XOR the [0]'th byte with RCon[i/4]
				tempWord.bytes[0] ^= Matrix_RCon[i / 4];

				//apply the addition (XOR?)of 't' aka tempWord
				for (int l = 0; l < 4; l++)
				{
					keyWords[i].bytes[l] = tempWord.bytes[l] ^ keyWords[i - 4].bytes[l];
				}
			}
		}
	}

	State test;
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			test.bytes[i][j] = Matrix_TestInput[i][j];
		}
	}

	cout << "---- BEFORE ANYTHING ----\n";
	test.print();

	Cipher(test, keyWords, 0, numRounds); 
	Cipher(test, keyWords, 1, numRounds);
	Cipher(test, keyWords, 2, numRounds); 
	Cipher(test, keyWords, 3, numRounds); 
	Cipher(test, keyWords, 4, numRounds); 
	Cipher(test, keyWords, 5, numRounds); 
	Cipher(test, keyWords, 6, numRounds);
	Cipher(test, keyWords, 7, numRounds);
	Cipher(test, keyWords, 8, numRounds);  
	Cipher(test, keyWords, 9, numRounds);
	Cipher(test, keyWords, 10, numRounds);

	cout << "---- AFTER CIPHER ----\n";
	test.print();

	Decrypt(test, keyWords, 0, numRounds);
	Decrypt(test, keyWords, 1, numRounds);
	Decrypt(test, keyWords, 2, numRounds);  
	Decrypt(test, keyWords, 3, numRounds);
	Decrypt(test, keyWords, 4, numRounds);
	Decrypt(test, keyWords, 5, numRounds); 
	Decrypt(test, keyWords, 6, numRounds); 
	Decrypt(test, keyWords, 7, numRounds);
	Decrypt(test, keyWords, 8, numRounds); 
	Decrypt(test, keyWords, 9, numRounds);
	Decrypt(test, keyWords, 10, numRounds);

	cout << "---- AFTER DECRYPT ----\n";
	test.print();

	return 0;
}

