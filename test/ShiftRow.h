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

#ifndef SHIFT_ROW_H
#define SHIFT_ROW_H

#include "State.h"

void ShiftRow(State &input);
void InvShiftRow(State &input);

#endif