// Filename: Cipher.h
// SIUE CS490-005
// Spring 2017
// Authors: David Shimkus and Michael Caponi
//
// NOTE: Byte operations are performed on char arrays.
//
// Revisions:
// 04/27/2017 | DS | Creation.  Moved from Main.cpp for program flow.

#ifndef CIPHER_H
#define CIPHER_H

void Cipher(State &input, Word *keyWords, int roundIndex, int numRounds);

void Decrypt(State &input, Word *keyWords, int roundIndex, int numRounds);

#endif