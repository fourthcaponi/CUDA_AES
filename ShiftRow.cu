// Filename: ShiftRow.cpp
// SIUE CS490-005
// Spring 2017
// Authors: David Shimkus and Michael Caponi
//
// NOTE: Byte operations are performed on char arrays.
//
// Revisions:
// 04/19/2017 | DS | Creation.
// 04/24/2017 | DS | Added the shift operators.
// 04/25/2017 | DS | Added the &'s to parameters.  Changed functions to return void.
// 05/01/2017 | MC | Convert to CUDA.

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <sys/stat.h> //for filesize

using namespace std;

#include "ShiftRow.h"
#include "State.h"

#include "cuda_runtime.h"
#include "device_launch_parameters.h"

__global__ void ShiftRow(int A[][4], int B[][4])
{
	int i = threadIdx.x;
	int j = threadIdx.y;

	if (i < 4)
	{
		unsigned char tempBytes[4];
		if (j < 4)
		{
			//perform the left shift as dependent upon the row
			tempBytes[j] = input.bytes[i][(j + i) % 4];
		}
		for (int k = 0; k < 4; k++)
		{
			input.bytes[i][k] = tempBytes[k];
		}
	}
}

__global__ void InvShiftRow(int A[][4], int B[][4])
{
	int offset = 4;
	int i = threadIdx.x;
	int j = threadIdx.y;

	if (i < 4)
	{
		int tempBytes[4];
		if (j < 4)
		{
			//perform the right shift as dependent upon the row
			tempBytes[j] = A.bytes[i][(j + offset) % 4];
		}
		for (int k = 0; k < 4; k++)
		{
			A.bytes[i][k] = tempBytes[k];
		}
		offset--;
	}

}