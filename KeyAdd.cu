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
// 05/01/2017 | MC | Convert to CUDA.

using namespace std;

#include "State.h"

#include "cuda_runtime.h"
#include "device_launch_parameters.h"

//note KeyAdd is the inverse of itself
__device__ void KeyAdd(State &input, Word(&keyWords)[60], size_t size, int round)
{
	int counter = round * 4; //appropriate offset

	int i = threadIdx.x;
	int j = threadIdx.y;

	if (i < 4)
	{
		if (j < 4)
		{

			//perform the XOR operation
			//note the order of [j] then [i]
			//input.bytes[j][i] ^= keyWords[counter].bytes[j];
			//temp = keyWords[counter].bytes[j];
			//input.bytes[j][i] ^= keyWords[counter].bytes[j];

			input.bytes[j][i] ^= keyWords[counter].bytes[j];
			
		}
		counter++;
	}
}