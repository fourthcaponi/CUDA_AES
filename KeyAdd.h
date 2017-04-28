// Filename: KeyAdd.h
// SIUE CS490-005
// Spring 2017
// Authors: David Shimkus and Michael Caponi
//
// NOTE: Byte operations are performed on char arrays.
//
// Revisions:
// 04/19/2017 | DS | Creation.
// 04/26/2017 | DS | Changed parameter pointer types.
// 04/27/2017 | DS | Changed block to state and keyWord to keyWords.

#ifndef KEY_ADD_H
#define KEY_ADD_H

#include "State.h"

//note KeyAdd is the inverse of itself
void KeyAdd(State &input, Word *keyWords, int round);

#endif