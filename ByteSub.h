// Filename: ByteSub.h
// SIUE CS490-005
// Spring 2017
// Authors: David Shimkus and Michael Caponi
//
// NOTE: Byte operations are performed on char arrays.
//
// Revisions:
// 04/19/2017 | DS | Creation.
// 05/01/2017 | MC | Convert to CUDA.

#ifndef BYTE_SUB_H
#define BYTE_SUB_H

#include "State.h"
#include "Matrices.h"

using namespace std;

__device__ void ByteSub(State &input);

__device__ void InvByteSub(State &input);

#endif