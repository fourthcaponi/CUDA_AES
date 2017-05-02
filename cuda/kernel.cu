#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <sys/stat.h> //for filesize
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <iomanip>

#include "cuda_runtime.h"
#include "device_launch_parameters.h"

using namespace std;

#define BLOCK_SIZE_BIT 128
#define BLOCK_SIZE_CHAR 16

typedef struct State
{
	int bytes[4][4];
}State;

struct block
{
	unsigned char text[BLOCK_SIZE_CHAR]; 
};


__global__ void ByteSub(int A[][4], int B[][4])
{

	const int Matrix_ByteSub[][16] = {
		{ 0x63, 0x7c, 0x77, 0x7b, 0xf2, 0x6b, 0x6f, 0xc5, 0x30, 0x01, 0x67, 0x2b, 0xfe, 0xd7, 0xab, 0x76 },
		{ 0xca, 0x82, 0xc9, 0x7d, 0xfa, 0x59, 0x47, 0xf0, 0xad, 0xd4, 0xa2, 0xaf, 0x9c, 0xa4, 0x72, 0xc0 },
		{ 0xb7, 0xfd, 0x93, 0x26, 0x36, 0x3f, 0xf7, 0xcc, 0x34, 0xa5, 0xe5, 0xf1, 0x71, 0xd8, 0x31, 0x15 },
		{ 0x04, 0xc7, 0x23, 0xc3, 0x18, 0x96, 0x05, 0x9a, 0x07, 0x12, 0x80, 0xe2, 0xeb, 0x27, 0xb2, 0x75 },
		{ 0x09, 0x83, 0x2c, 0x1a, 0x1b, 0x6e, 0x5a, 0xa0, 0x52, 0x3b, 0xd6, 0xb3, 0x29, 0xe3, 0x2f, 0x84 },
		{ 0x53, 0xd1, 0x00, 0xed, 0x20, 0xfc, 0xb1, 0x5b, 0x6a, 0xcb, 0xbe, 0x39, 0x4a, 0x4c, 0x58, 0xcf },
		{ 0xd0, 0xef, 0xaa, 0xfb, 0x43, 0x4d, 0x33, 0x85, 0x45, 0xf9, 0x02, 0x7f, 0x50, 0x3c, 0x9f, 0xa8 },
		{ 0x51, 0xa3, 0x40, 0x8f, 0x92, 0x9d, 0x38, 0xf5, 0xbc, 0xb6, 0xda, 0x21, 0x10, 0xff, 0xf3, 0xd2 },
		{ 0xcd, 0x0c, 0x13, 0xec, 0x5f, 0x97, 0x44, 0x17, 0xc4, 0xa7, 0x7e, 0x3d, 0x64, 0x5d, 0x19, 0x73 },
		{ 0x60, 0x81, 0x4f, 0xdc, 0x22, 0x2a, 0x90, 0x88, 0x46, 0xee, 0xb8, 0x14, 0xde, 0x5e, 0x0b, 0xdb },
		{ 0xe0, 0x32, 0x3a, 0x0a, 0x49, 0x06, 0x24, 0x5c, 0xc2, 0xd3, 0xac, 0x62, 0x91, 0x95, 0xe4, 0x79 },
		{ 0xe7, 0xcb, 0x37, 0x6d, 0x8d, 0xd5, 0x4e, 0xa9, 0x6c, 0x56, 0xf4, 0xea, 0x65, 0x7a, 0xae, 0x08 },
		{ 0xba, 0x78, 0x25, 0x2e, 0x1c, 0xa6, 0xb4, 0xc6, 0xe8, 0xdd, 0x74, 0x1f, 0x4b, 0xbd, 0x8b, 0x8a },
		{ 0x70, 0x3e, 0xb5, 0x66, 0x48, 0x03, 0xf6, 0x0e, 0x61, 0x35, 0x57, 0xb9, 0x86, 0xc1, 0x1d, 0x9e },
		{ 0xe1, 0xf8, 0x98, 0x11, 0x69, 0xd9, 0x8e, 0x94, 0x9b, 0x1e, 0x87, 0xe8, 0xce, 0x55, 0x28, 0xdf },
		{ 0x8c, 0xa1, 0x89, 0x0d, 0xbf, 0xe6, 0x42, 0x68, 0x41, 0x99, 0x2d, 0x0f, 0xb0, 0x54, 0xbb, 0x16 }
	};

	int i = threadIdx.x;
	int j = threadIdx.y;

	if (i < 4)
	{
		if (j < 4)
		{
			//get the ASCII value as well as its decimal value
			int byteTemp = A[i][j];

			//get the leftmost 4 bits aka the COLUMN
			byteTemp = A[i][j];
			byteTemp = (byteTemp >> 4) & ((1 << 4) - 1); //leftmost 4 bits
			int column = byteTemp;

			//get the rightmost 4 bits aka the ROW
			byteTemp = A[i][j];
			byteTemp = (byteTemp >> 0) & ((1 << 4) - 1); //rightmost 4 bits
			int row = byteTemp;

			//set the original bytes on the passed in matrix to the new bytes
			B[i][j] = Matrix_ByteSub[column][row];
		}
	}
}

__global__ void ShiftRow(int A[][4], int B[][4])
{

	//NEED TO GET A INTO B


	int i = threadIdx.x;
	int j = threadIdx.y;
	int k = threadIdx.y;

	


	if (i < 4)
	{
		unsigned char tempBytes[4];
		if (j < 4)
		{
			//perform the left shift as dependent upon the row
			tempBytes[j] = A[i][(j + i) % 4];
			//B[i][j] = tempBytes[j];
		}
		if (k < 4)
		{
			A[i][k] = tempBytes[k];
			//B[i][k] = A[i][k];
		}
	}

	for(int m = 0; m < 4; m ++)
	{
		for(int n = 0; n < 4; n++)
		{

			B[m][n]=A[m][n];
		}
	}
}


__device__ unsigned char gmul(unsigned char a, unsigned char b)
{
	//Original Author: Sam Trenholme

	//GF(2^8) multiplication/addition
	unsigned char p = 0; // the product of the multiplication
	unsigned char counter;
	unsigned char hi_bit_set;
	for (counter = 0; counter < 8; counter++)
	{
		// if b is odd, then add (XOR) the corresponding a to p (final product = sum of all a's corresponding to odd b's)
		if ((b & 1) == 1)
		{
			p ^= a;
		}

		// GF modulo: if a >= 128, then it will overflow when shifted left, so reduce
		hi_bit_set = (a & 0x80);
		a <<= 1;

		// if a's hi bit had a value of one prior to this rotation, exclusive or a with the hexadecimal number 0x1b
		// 0x1b = x^4 + x^3 + x + 1 of GF(2^8) irreducible polynomial
		if (hi_bit_set == 0x80)
		{
			a ^= 0x1b;
		}

		// rotate b one bit to the right, discarding the low bit, and making the high (eighth from left) bit have a value of zero		
		b >>= 1;
	}
	return p;
}

__global__ void MixColumn(int A[][4], int B[][4])
{
	unsigned char col[4];
	unsigned char copyCol[4];

	int i = threadIdx.x;
	//int j = threadIdx.y;
	//int k = threadIdx.y;

	/*
	if (i < 4)
	{
		if (j < 4)
		{
			col[j] = A[j][i];
		}

		// apply the mixColumn on one column 
		unsigned char copyCol[4];

		if (k < 4)
		{
			copyCol[k] = col[k];
		}

		col[0] = gmul(copyCol[0], 2) ^ gmul(copyCol[1], 3) ^ gmul(copyCol[2], 1) ^ gmul(copyCol[3], 1);
		col[1] = gmul(copyCol[0], 1) ^ gmul(copyCol[1], 2) ^ gmul(copyCol[2], 3) ^ gmul(copyCol[3], 1);
		col[2] = gmul(copyCol[0], 1) ^ gmul(copyCol[1], 1) ^ gmul(copyCol[2], 2) ^ gmul(copyCol[3], 3);
		col[3] = gmul(copyCol[0], 3) ^ gmul(copyCol[1], 1) ^ gmul(copyCol[2], 1) ^ gmul(copyCol[3], 2);

		// put the values back into the state 
		if (j < 4)
		{
			B[j][i] = col[j];
		}
	}
	*/

	if (i < 4)
	{
		for(int m = 0; m < 4; m++)
		{
			copyCol[m] = A[m][i];
		}

		// apply the mixColumn on one column 

		col[0] = gmul(copyCol[0], 2) ^ gmul(copyCol[1], 3) ^ gmul(copyCol[2], 1) ^ gmul(copyCol[3], 1);
		col[1] = gmul(copyCol[0], 1) ^ gmul(copyCol[1], 2) ^ gmul(copyCol[2], 3) ^ gmul(copyCol[3], 1);
		col[2] = gmul(copyCol[0], 1) ^ gmul(copyCol[1], 1) ^ gmul(copyCol[2], 2) ^ gmul(copyCol[3], 3);
		col[3] = gmul(copyCol[0], 3) ^ gmul(copyCol[1], 1) ^ gmul(copyCol[2], 1) ^ gmul(copyCol[3], 2);

		// put the values back into the state 
		for(int n = 0; n < 4; n++)
		{
			B[n][i] = col[n];
		}
	}
}


int main()
{
	int i, j;
	dim3 threadsPerBlock(4, 4);

	 int test_matrix[4][4] = {
		{ 0x00, 0x12, 0x0c, 0x08 },
		{ 0x04, 0x04, 0x00, 0x23 },
		{ 0x12, 0x12, 0x13, 0x19 },
		{ 0x14, 0x00, 0x11, 0x19 }
	 };

	State test;
	for (int i = 0; i < 4; i++) {
		for (int j  =0; j < 4; j++) {
			test.bytes[i][j] = test_matrix[i][j];
		}
	}

	int BS[4][4];
	int SR[4][4];
	int MC[4][4];

	int(*d_test)[4];
	int(*d_BS)[4];
	int(*d_SR)[4];
	int(*d_MC)[4];

	cudaMalloc((void**)&d_test, (4 * 4) * sizeof(int));
	cudaMalloc((void**)&d_BS, (4 * 4) * sizeof(int));
	cudaMalloc((void**)&d_SR, (4 * 4) * sizeof(int));
	cudaMalloc((void**)&d_MC, (4 * 4) * sizeof(int));

	cudaMemcpy(d_test, &test, (4 * 4) * sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(d_BS, BS, (4 * 4) * sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(d_SR, SR, (4 * 4) * sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(d_MC, MC, (4 * 4) * sizeof(int), cudaMemcpyHostToDevice);

	/*------------------BYTESUB--------------------*/
	ByteSub <<<4, threadsPerBlock >>>(d_test, d_BS);

	cudaMemcpy(&test, d_test, (4 * 4) * sizeof(int), cudaMemcpyDeviceToHost);
	cudaMemcpy(BS, d_BS, (4 * 4) * sizeof(int), cudaMemcpyDeviceToHost);

	/*------------------SHIFTROW--------------------*/
	State bytesub;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			bytesub.bytes[i][j] = BS[i][j];
		}
	}

	int(*d_bytesub)[4];
	cudaMalloc((void**)&d_bytesub, (4 * 4) * sizeof(int));

	cudaMemcpy(d_bytesub, &bytesub, (4 * 4) * sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(d_SR, SR, (4 * 4) * sizeof(int), cudaMemcpyHostToDevice);

	ShiftRow<<<4, threadsPerBlock >>>(d_bytesub, d_SR);

	cudaMemcpy(&bytesub, d_bytesub, (4 * 4) * sizeof(int), cudaMemcpyDeviceToHost);
	cudaMemcpy(SR, d_SR, (4 * 4) * sizeof(int), cudaMemcpyDeviceToHost);

	/*------------------MIXCOLUMN--------------------*/
	State shiftrow;
	for (int i = 0; i < 4; i++) {
		for (int j = 0; j < 4; j++) {
			shiftrow.bytes[i][j] = SR[i][j];
		}
	}

	int(*d_shiftrow)[4];
	cudaMalloc((void**)&d_shiftrow, (4 * 4) * sizeof(int));

	cudaMemcpy(d_shiftrow, &shiftrow, (4 * 4) * sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(d_SR, SR, (4 * 4) * sizeof(int), cudaMemcpyHostToDevice);

	MixColumn <<<4, threadsPerBlock >>>(d_shiftrow, d_MC);

	cudaMemcpy(&shiftrow, d_shiftrow, (4 * 4) * sizeof(int), cudaMemcpyDeviceToHost);
	cudaMemcpy(MC, d_MC, (4 * 4) * sizeof(int), cudaMemcpyDeviceToHost);


	printf("test = \n");
	for (i = 0; i<4; i++) {
		for (j = 0; j<4; j++) {
			//printf("%d ", test.bytes[i][j]);
			cout << hex << test.bytes[i][j] << endl;
		}
		printf("\n\n");
	}

	printf("BS = \n");
	for (i = 0; i<4; i++) {
		for (j = 0; j<4; j++) {
			//printf("%d ", BS[i][j]);
			cout << hex << BS[i][j] << endl;
		}
		printf("\n\n");
	}

	printf("SR = \n");
	for (i = 0; i<4; i++) {
		for (j = 0; j<4; j++) {
			//printf("%d ", SR[i][j]);
			cout << hex << SR[i][j] << endl;
		}
		printf("\n\n");
	}

	printf("MC = \n");
	for (i = 0; i<4; i++) {
		for (j = 0; j<4; j++) {
			//printf("%d ", MC[i][j]);
			cout << hex << MC[i][j] << endl;
		}
		printf("\n\n");
	}

	cudaFree(d_test);
	cudaFree(d_BS);
	cudaFree(d_SR);
	cudaFree(d_MC);

	return 0;
}
