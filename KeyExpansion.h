// Filename: KeyExpansion.h
// SIUE CS490-005
// Spring 2017
// Authors: David Shimkus and Michael Caponi
//
// NOTE: Byte operations are performed on char arrays.
//
// Revisions:
// 04/26/2017 | DS | Creation.
// 05/01/2017 | MC | Convert to CUDA.

#ifndef KEY_EXP_H
#define KEY_EXP_H

using namespace std;

__device__ void RotWord(Word &word);
__device__ void SubWord(Word &word);
__global__ void ke_128(Word *keyWords, int numKeyWords);
__global__ void ke_192(Word *keyWords, int numKeyWords);
__global__ void ke_256(Word *keyWords, int numKeyWords);

#endif