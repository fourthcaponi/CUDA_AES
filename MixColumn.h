// Filename: MixColumn.h
// SIUE CS490-005
// Spring 2017
// Authors: David Shimkus and Michael Caponi
//
// NOTE: Byte operations are performed on char arrays.
//
// Revisions:
// 04/19/2017 | DS | Creation.

#ifndef MIX_COL_H
#define MIX_COL_H

#include "State.h"


void MixColumn(State input);
void mc_operation(unsigned char *col);
void InvMixColumn (State input);
void mc_operation(unsigned char *col);

#endif