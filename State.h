// Filename: State.h
// SIUE CS490-005
// Spring 2017
// Authors: David Shimkus and Michael Caponi
//
// NOTE: Byte operations are performed on char arrays.
//
// Revisions:
// 04/19/2017 | DS | Creation.

#ifndef STATE_H
#define STATE_H

struct State{

    unsigned char bytes[4][4] = {{0x63, 0xc9, 0xfe, 0x30},
	                            {0xf2, 0x63, 0x26, 0xf2},
	                            {0x7d, 0xd4, 0xc9, 0xc9},
	                            {0xd4, 0xfa, 0x63, 0x82}};

};

#endif