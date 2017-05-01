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
#define N 512;
#define GRID_SIZE 1;



struct State
{
	unsigned char bytes[4][4];
	int ByteSub_values[4][4];

	void print()
	{
		cout << endl;
		cout << "{ { " << setfill('0') << setw(2) << hex << (int)bytes[0][0] << ", " << setfill('0') << setw(2) << hex << (int)bytes[0][1] << ", " << setfill('0') << setw(2) << hex << (int)bytes[0][2] << ", " << setfill('0') << setw(2) << hex << (int)bytes[0][3] << " },\n";
		cout << "  { " << setfill('0') << setw(2) << hex << (int)bytes[1][0] << ", " << setfill('0') << setw(2) << hex << (int)bytes[1][1] << ", " << setfill('0') << setw(2) << hex << (int)bytes[1][2] << ", " << setfill('0') << setw(2) << hex << (int)bytes[1][3] << " },\n";
		cout << "  { " << setfill('0') << setw(2) << hex << (int)bytes[2][0] << ", " << setfill('0') << setw(2) << hex << (int)bytes[2][1] << ", " << setfill('0') << setw(2) << hex << (int)bytes[2][2] << ", " << setfill('0') << setw(2) << hex << (int)bytes[2][3] << " },\n";
		cout << "  { " << setfill('0') << setw(2) << hex << (int)bytes[3][0] << ", " << setfill('0') << setw(2) << hex << (int)bytes[3][1] << ", " << setfill('0') << setw(2) << hex << (int)bytes[3][2] << ", " << setfill('0') << setw(2) << hex << (int)bytes[3][3] << " } }\n\n";
	}

	void printAscii()
	{
		cout << endl;
		cout << "{ { " << (char)bytes[0][0] << ", " << (char)bytes[0][1] << ", " << (char)bytes[0][2] << ", " << (char)bytes[0][3] << " },\n";
		cout << "  { " << (char)bytes[1][0] << ", " << (char)bytes[1][1] << ", " << (char)bytes[1][2] << ", " << (char)bytes[1][3] << " },\n";
		cout << "  { " << (char)bytes[2][0] << ", " << (char)bytes[2][1] << ", " << (char)bytes[2][2] << ", " << (char)bytes[2][3] << " },\n";
		cout << "  { " << (char)bytes[3][0] << ", " << (char)bytes[3][1] << ", " << (char)bytes[3][2] << ", " << (char)bytes[3][3] << " } }\n\n";

	}
};

struct block
{
	unsigned char text[BLOCK_SIZE_CHAR]; //divide by 8 b/c 1 char = 1 byte
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

int main()
{

	State A = {
		{ 0x00, 0x12, 0x0c, 0x08 },
		{ 0x04, 0x04, 0x00, 0x23 },
		{ 0x12, 0x12, 0x13, 0x19 },
		{ 0x14, 0x00, 0x11, 0x19 }
	};

	int B[4][4];

	int(*pA)[4];
	int(*pB)[4];
	cudaMalloc((void**)&pA, (4*4) * sizeof(int));
	cudaMalloc((void**)&pB, (4*4) * sizeof(int));


	cudaMemcpy(pA, A, (4*4) * sizeof(int), cudaMemcpyHostToDevice);
	cudaMemcpy(pB, B, (4 * 4) * sizeof(int), cudaMemcpyHostToDevice);

	int numBlocks = 1;
	dim3 threadsPerBlock(4, 4);
	ByteSub<<<4, threadsPerBlock>>>(pA, pB);

	cudaMemcpy(A, pA, (4*4) * sizeof(int), cudaMemcpyDeviceToHost);
	cudaMemcpy(B, pB, (4 * 4) * sizeof(int), cudaMemcpyDeviceToHost);


	int i, j; printf("B = \n");
	for (i = 0; i<4; i++) {
		for (j = 0; j<4; j++) {
			printf("%d ", B[i][j]);
		}
		printf("\n");
	}

	cudaFree(pA);
	cudaFree(pB);

    return 0;
}
