// Filename: MixColumn.h
// SIUE CS490-005
// Spring 2017
// Authors: David Shimkus and Michael Caponi
//
// NOTE: Byte operations are performed on char arrays.
//
// Revisions:
// 04/19/2017 | DS | Creation.
// 04/23/2017 | MC | Included mc_operation
// 04/25/2017 | DS | Renamed the second mc_operation to inv_mc_operation.  Placed & into parameters.

#ifndef MIX_COL_H
#define MIX_COL_H

#include "State.h"
#include "cuda_runtime.h"
#include "device_launch_parameters.h"

__global__ void MixColumn(State &input);
__global__ void InvMixColumn(State &input);

//TODO: fix naming parameters
//unsigned char GaloisMult(int operand, int power);

__global__ void mc_operation(unsigned char *col);
__global__ void inv_mc_operation(unsigned char *col);

#endif