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

#ifndef SHIFT_ROW_H
#define SHIFT_ROW_H

#include "State.h"


State ShiftRow(State input);
State InvShiftRow(State input);

#endif