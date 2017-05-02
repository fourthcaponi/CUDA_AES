// Filename: Cipher.h
// SIUE CS490-005
// Spring 2017
// Authors: David Shimkus and Michael Caponi
//
// NOTE: Byte operations are performed on char arrays.
//
// Revisions:
// 04/27/2017 | DS | Creation.  Moved from Main.cpp for program flow.
// 04/28/2017 | DS | Include State.h
// 04/29/2017 | DS | Changed the way the keyWords was getting passed in.
// 05/01/2017 | MC | Convert to CUDA.

#ifndef CIPHER_H
#define CIPHER_H

#include "State.h"

__global__ void Cipher(State &input, Word (&keyWords)[60], size_t size, int roundIndex, int numRounds);

__global__ void Decrypt(State &input, Word(&keyWords)[60], size_t size, int roundIndex, int numRounds);

#endif