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
// 04/26/2017 | DS | Code cleanup.
// 05/01/2017 | MC | Convert to CUDA.

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>

using namespace std;

#include "ByteSub.h"
#include "State.h"
#include "Matrices.h"
#include "KeyExpansion.h"

#include "cuda_runtime.h"
#include "device_launch_parameters.h"

__device__ void ByteSub(State &input)
{
	int i = threadIdx.x;
	int j = threadIdx.y;

	if (i < 4)
	{
		if (j < 4)
		{
			//get the ASCII value as well as its decimal value
			int byteTemp = input.bytes[i][j];

			//get the leftmost 4 bits aka the COLUMN
			byteTemp = input.bytes[i][j];
			byteTemp = (byteTemp >> 4) & ((1 << 4) - 1); //leftmost 4 bits
			int column = byteTemp;

			//get the rightmost 4 bits aka the ROW
			byteTemp = input.bytes[i][j];
			byteTemp = (byteTemp >> 0) & ((1 << 4) - 1); //rightmost 4 bits
			int row = byteTemp;

			//set the original bytes on the passed in matrix to the new bytes
			input.bytes[i][j] = Matrix_ByteSub[column][row];
		}
	}
}

__device__ void InvByteSub(State &input)
{
	int i = threadIdx.x;
	int j = threadIdx.y;

	if (i < 4)
	{
		if (j < 4)
		{
			//get the ASCII value as well as its decimal value
			int byteTemp = input.bytes[i][j];

			//get the leftmost 4 bits aka the COLUMN
			byteTemp = input.bytes[i][j];
			byteTemp = (byteTemp >> 4) & ((1 << 4) - 1); //leftmost 4 bits
			int column = byteTemp;

			//get the rightmost 4 bits aka the ROW
			byteTemp = input.bytes[i][j];
			byteTemp = (byteTemp >> 0) & ((1 << 4) - 1); //rightmost 4 bits
			int row = byteTemp;

			//set the original bytes on the passed in matrix to the new bytes
			input.bytes[i][j] = Matrix_InvByteSub[column][row];
		}
	}
}

