// Filename: ShiftRow.h
// SIUE CS490-005
// Spring 2017
// Authors: David Shimkus and Michael Caponi
//
// NOTE: Byte operations are performed on char arrays.
//
// Revisions:
// 04/19/2017 | DS | Creation.
// 04/24/2017 | DS | Added InvShiftRow.
// 04/25/2017 | DS | Added the &'s to parameters. Changed functions to return void.
// 05/01/2017 | MC | Convert to CUDA.

#ifndef SHIFT_ROW_H
#define SHIFT_ROW_H

#include "State.h"

#include "cuda_runtime.h"
#include "device_launch_parameters.h"

__global__ void ShiftRow(int A[][4], int B[][4]);
__global__ void InvShiftRow(int A[][4], int B[][4]);

#endif