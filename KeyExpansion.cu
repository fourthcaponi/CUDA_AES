// Filename: KeyExpansion.cpp
// SIUE CS490-005
// Spring 2017
// Authors: David Shimkus and Michael Caponi
//
// NOTE: Byte operations are performed on char arrays.
//
// Revisions:
// 04/26/2017 | DS | Creation.
// 05/01/2017 | MC | cuda implementation
// 05/01/2017 | MC | Convert to CUDA.

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>

#include "State.h"
#include "Matrices.h"

#include "cuda_runtime.h"
#include "device_launch_parameters.h"

using namespace std;

__device__ void RotWord(Word &word)
{
	unsigned char tempBytes[4];
	for (int i = 0; i < 4; i++)
	{
		//place the appropriate left shifted bytes into the temp variable
		tempBytes[i] = word.bytes[(i + 1) % 4];
	}
	for (int k = 0; k < 4; k++)
	{
		//place the temp var's data into the original input word
		word.bytes[k] = tempBytes[k];
	}
}

__device__ void SubWord(Word &word)
{
	for (int i = 0; i < 4; i++)
	{
		//get the ASCII value as well as its decimal value
		int byteTemp = word.bytes[i];

		//get the leftmost 4 bits aka the COLUMN
		byteTemp = (byteTemp >> 4) & ((1 << 4) - 1); //leftmost 4 bits
		int column = byteTemp;

		//get the rightmost 4 bits aka the ROW
		byteTemp = word.bytes[i];
		byteTemp = (byteTemp >> 0) & ((1 << 4) - 1); //rightmost 4 bits
		int row = byteTemp;

		//set the original bytes on the passed in matrix to the new bytes
		word.bytes[i] = Matrix_Rijndael[column][row];
	}
}

__global__ void ke_128(Word *keyWords, int numKeyWords) 
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

__global__ void ke_192(Word *keyWords, int numKeyWords)
{
	//note: this is basically the same as 128 bit except for 6 word "groups" instead of 4
	Word tempWord;
	for (int i = 4; i < numKeyWords; i++)
	{
		if (i % 6 != 0)
		{
			for (int j = 0; j < 4; j++)
			{
				//note the keyWords[i-6] here instead of [i-4]
				keyWords[i].bytes[j] = keyWords[i - 1].bytes[j] ^ keyWords[i - 6].bytes[j];
			}
		}
		else
		{
			//function g
			for (int k = 0; k < 4; k++)
			{
				//note 'tempword' is the 't' variable from the book
				tempWord.bytes[k] = keyWords[i - 1].bytes[k]; //initially populate the tempword		
			}
			//perform the rotation
			RotWord(tempWord);
			//apply the subword
			SubWord(tempWord);

			//XOR the [0]'th byte it with Rconi/4
			tempWord.bytes[0] ^= Matrix_RCon[i / 4];

			//apply the addition (XOR?)of 't' aka tempWord
			for (int l = 0; l < 4; l++)
			{
				keyWords[i].bytes[l] = tempWord.bytes[l] ^ keyWords[i - 6].bytes[l];
			}
		}
	}
}

__global__ void ke_256(Word *keyWords, int numKeyWords) 
{

	

	Word tempWord;
	if (int i = 8; i < numKeyWords; i++)
	{
		if (i % 8 != 0)
		{
			//check for the additional step needed for 256
			//this is subsection 2.c. of pg 212 from the textbook
			if (i % 4 == 0)
			{
				for (int j = 0; j < 4; j++)
				{
					tempWord.bytes[j] = keyWords[i - 1].bytes[j];
				}
				//perform the SubWord
				SubWord(tempWord);

				for (int j = 0; j < 4; j++)
				{
					tempWord.bytes[j] ^= keyWords[i - 8].bytes[j];
				}
			}
			else
			{
				for (int j = 0; j < 4; j++)
				{
					keyWords[i].bytes[j] = keyWords[i - 1].bytes[j] ^ keyWords[i - 8].bytes[j];

				}
			}
		}
		else
		{
			for (int k = 0; k < 4; k++)
			{
				tempWord.bytes[k] = keyWords[i - 1].bytes[k]; //initially populate the tempword		
			}
			//perform the rotation
			RotWord(tempWord);
			//apply the subword
			SubWord(tempWord);
			//XOR it with Rcon[i/4]
			for (int k = 0; k < 4; k++)
			{
				tempWord.bytes[k] ^= Matrix_RCon[i / 4];
			}
			//apply the addition (XOR?)of 't' aka tempWord
			for (int l = 0; l < 4; l++)
			{
				keyWords[i].bytes[l] = tempWord.bytes[l] ^ keyWords[i - 8].bytes[l];
			}
		}
	}
}