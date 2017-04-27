// Filename: main.cpp
// SIUE CS490-005
// Spring 2017
// Authors: David Shimkus and Michael Caponi
//
// NOTE: Byte operations are performed on char arrays.
//
// Revisions:
// 04/10/2017 | DS | Creation.  I/O and chopping message into chunks set up.
// 04/12/2017 | DS | Worked on the 'chunks' portion more.  BLOCK_SIZE char vs bit fixed.
// 04/24/2017 | DS | Plugged in ByteSub and InvByteSub things.
// 04/25/2017 | DS | Added the new print() function after each state.  Using test matrix.
// 04/26/2017 | DS | Added in the key input, numRounds stuff.  Code cleanup.
// 04/27/2017 | DS | Work on the key expansion.  KeyWords[0] to KeyWords[groupsize] working.
// 04/27/2017 | DS | Adding appropriate keyWords to keys (array of states).

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <sys/stat.h> //for filesize
#include <vector>
#include <stdlib.h>
#include <stdio.h>
#include <iomanip>

//the external files
#include "ByteSub.h"
#include "ShiftRow.h"
#include "MixColumn.h"
#include "KeyAdd.h"
#include "State.h"
#include "Matrices.h" //for the test input matrix
#include "KeyExpansion.h"

using namespace std;

#define BLOCK_SIZE_BIT 128
#define BLOCK_SIZE_CHAR 16   //a single character is stored as a byte, so 16 bytes = 128 bits

//this struct for the array of blocks implementation
struct block
{
	char text[BLOCK_SIZE_CHAR]; //divide by 8 b/c 1 char = 1 byte
};

//TODO: accept keysize, filenames as arguments
int main()
{
	ifstream message, key;
	ofstream ciphertext;
	string messageName, keyName;
	int keySize 	= -1; //126, 192, or 256
	int numRounds 	= -1; //dependent on the keysize
	int numKeys 	= -1; //includes 'groupsize' number of keywords
	int numKeyWords = -1; //for the key expansion
	int groupSize 	= -1; //the number of 'words' in each group of key expansions

	printf("\nAES\n");
	printf("\nBy David Shimkus and Michael Caponi\n\n");
	
	//TODO: if arguments are passed in do not do this input/output printf stuff

	//TODO: grab the PATH not just the filename (so can look at entire system not just local directory)
	printf("Please enter the input message filename: ");
	cin >> messageName;

	message.open(messageName.c_str());
	//TODO: file i/o error handling

	int selection;
	bool done = false;
	while (!done)
	{
		printf("\nWhat is the size of the key?");
		printf("\n1. 128 bit");
		printf("\n2. 192 bit");
		printf("\n3. 256 bit");
		printf("\nSelection: ");

		cin >> selection;

		switch (selection)
		{
		case  1:
		{
			keySize   = 128; 
			numRounds = 10; 
			numKeys   = 11;  //could also be numRounds + 1 (?)
			numKeyWords  = 44;
			groupSize = 4;
			break;
		} 
		case  2:
		{
			keySize   = 192; 
			numRounds = 12; 
			numKeys   = 13;
			numKeyWords  = 52;
			groupSize = 6;
			break;
		} 
		case  3: 
		{
			keySize   = 256; 
			numRounds = 14; 
			numKeys   = 15;
			numKeyWords  = 60;
			groupSize = 8;
			break;
		}
		default: keySize = -1; //bad input
		}

		if (keySize == -1)
		{
			printf("\nERROR: please select an appropriate key size...\n");
		}
		else
		{
			//TODO: grab the PATH not just the filename (so can look at entire system not just local directory)
			printf("\nPlease enter the key filename: ");
			cin >> keyName;

			key.open(keyName.c_str());
			//TODO: file i/o error handling

			//make sure the size of the key file matches the selection
			struct stat st;
			stat(keyName.c_str(), &st);
			if (st.st_size * 8 != keySize)
			{
				printf("\nERROR: the key filesize does NOT match the selection above.\n");
			}
			else
			{
				done = true;
			}
		}
	}

	//find message length in CHARACTERS
	int messageSizeChar = 0;
	string line;
	while(getline(message, line))
	{
		messageSizeChar += line.length() + 1;//add 1 for the newline character
	}

	//find out how many blocks we will need
	double	numBlocksD = (double)messageSizeChar / (double)BLOCK_SIZE_CHAR;
	int		numBlocks  = messageSizeChar / BLOCK_SIZE_CHAR;
	if (numBlocksD > numBlocks)
	{
		//if there is a 'fraction' of a whole block we still need an extra block
		numBlocks++;
	}

	//return to the beginning of the message stream
	message.clear();
	message.seekg(0, ios::beg);

	//actually grab the contents and place into a string
	stringstream buffer;
	buffer << message.rdbuf();
	string messageString = buffer.str();

	//copy the 'string' variable into a character array
	char messageChars[messageSizeChar + 1];
	strncpy(messageChars, messageString.c_str(), sizeof(messageChars));
	messageChars[messageSizeChar + 1] = '\0'; //add this null guy to the end

	//split the message into chunks
	block blocks[numBlocks];
	int counter = 0;
	for(int i = 0; i < numBlocks; i++)
	{
		counter = i * BLOCK_SIZE_CHAR;
		for(int j = 0; j < BLOCK_SIZE_CHAR; j++)
		{
			blocks[i].text[j] = messageChars[counter];
			counter++;
		}
	}

	//OK so at this point the 'blocks' variable contains an array
	//of struct block's which contain the '.text' field that can
	//be passed to different portions of AES as appropriate
	//i.e. if we are going to pass the 3rd 'block' to AES it would
	//be the 3rd chunk of 128 bits i.e. 16 characters i.e. blocks[2].text

	//printf("%.*s\n", BLOCK_SIZE_CHAR, blocks[2].text);
	//printf(blocks[2].text);

	//now we need to get the input key - we will need it for key expansion
	int keySizeChar = keySize / 8; //divide by 8 to convert bits to bytes

	//return to the beginning of the key stream
	key.clear();
	key.seekg(0, ios::beg);

	//actually grab the contents and place into a string
	stringstream buffer2;
	buffer2 << key.rdbuf();
	string keyString = buffer2.str();

	//copy the 'string' variable into a character array
	char keyChars[keySizeChar + 1];
	
	//TODO: uncomment the following line to get the input key.txt
	//strncpy(keyChars, keyString.c_str(), sizeof(keyChars));

	for(int i = 0; i < keySizeChar; i++)
	{
		keyChars[i] = Matrix_TestKey[i];
	}

	keyChars[keySizeChar + 1] = '\0'; //add this null guy to the end

	//declare our array of keys that will be populated with the key expansions
	Word keyWords[numKeyWords]; //the total "words" to be populated

	//TODO: remove the following???
	State keys[numKeys]; //a "state" consists of 4 words i.e. 16 bytes total

	//perform the initial population for keyWords[0] to keyWords[groupSize] 
	int offset = 0;
	for(int i = 0; i < groupSize; i++)
	{
		for(int j = offset; j < offset + 4; j++)
		{
			//TODO: does this need to be 4*i???
			keyWords[i].bytes[j%4] = keyChars[j]; 
		}
		offset += 4;
	}

	//compute the rest of the keyWords
	//TODO: there is probably a better way to do this (use groupsize and %'s)
	if(keySize == 128)
	{
		Word tempWord;
		for(int i = 4; i < numKeyWords; i++)
		{
			if(i%4 != 0)
			{
				for(int j = 0; j < 4; j++)
				{
					keyWords[i].bytes[j] = keyWords[i-1].bytes[j] ^ keyWords[i-4].bytes[j];
				}
			}
			else
			{
				//function g
				for(int k = 0; k < 4; k++)
				{
					tempWord.bytes[k] = keyWords[i-1].bytes[k]; //initially populate the tempword		
				}
				//perform the rotation
				RotWord(tempWord);
				//apply the subword
				SubWord(tempWord);

				//XOR the [0]'th byte it with Rconi/4
				tempWord.bytes[0] ^= Matrix_RCon[i/4];
				
				//apply the addition (XOR?)of 't' aka tempWord

				cout << "---- the t value for round " << i/4<<" ----\n";
				tempWord.print();

				for(int l = 0; l < 4; l++)
				{
					keyWords[i].bytes[l] = tempWord.bytes[l] ^ keyWords[i-4].bytes[l];
				}
			}	
		}
	}
	else if(keySize == 192)
	{
		Word tempWord;
		for(int i = 4; i < numKeyWords; i++)
		{
			if(i%6 != 0)
			{
				for(int j = 0; j < 4; j++)
				{
					keyWords[i].bytes[j] = keyWords[i-1].bytes[j] ^ keyWords[i-6].bytes[j];
				}
			}
			else
			{
				//function g
				for(int k = 0; k < 4; k++)
				{
					//note 'tempword' is the 't' variable from the book
					tempWord.bytes[k] = keyWords[i-1].bytes[k]; //initially populate the tempword		
				}
				//perform the rotation
				RotWord(tempWord);
				//apply the subword
				SubWord(tempWord);

				//XOR the [0]'th byte it with Rconi/4
				tempWord.bytes[0] ^= Matrix_RCon[i/4];
				
				//apply the addition (XOR?)of 't' aka tempWord

				cout << "---- the t value for round " << i/4<<" ----\n";
				tempWord.print();

				for(int l = 0; l < 4; l++)
				{
					keyWords[i].bytes[l] = tempWord.bytes[l] ^ keyWords[i-6].bytes[l];
				}
			}	
		}
	}
	else if(keySize == 256)
	{
		Word tempWord;
		for(int i = 8; i < numKeyWords; i++)
		{
			if(i%8 != 0)
			{
				//check for the additional step needed for 256
				//this is subsection 2.c. of pg 212 from the textbook
				if(i%4 == 0)
				{
					for(int j = 0; j < 4; j++)
					{
						tempWord.bytes[j] = keyWords[i-1].bytes[j];
					}
					//perform the SubWord
					SubWord(tempWord);

					for(int j = 0; j < 4; j++)
					{
						tempWord.bytes[j] ^= keyWords[i-8].bytes[j];
					}
				}
				else
				{
					for(int j = 0; j < 4; j++)
					{
						keyWords[i].bytes[j] = keyWords[i-1].bytes[j] ^ keyWords[i-8].bytes[j];

					}
				}
			}
			else
			{
				for(int k = 0; k < 4; k++)
				{
					tempWord.bytes[k] = keyWords[i-1].bytes[k]; //initially populate the tempword		
				}
				//perform the rotation
				RotWord(tempWord);
				//apply the subword
				SubWord(tempWord);
				//XOR it with Rcon[i/4]
				for(int k = 0; k < 4; k++)
				{
					tempWord.bytes[k] ^= Matrix_RCon[i/4];
				}
				//apply the addition (XOR?)of 't' aka tempWord
				for(int l = 0; l < 4; l++)
				{
					keyWords[i].bytes[l] = tempWord.bytes[l] ^ keyWords[i-8].bytes[l];
				}
			}	
		}
	}
	else
	{
		//something went wrong...
	}
	
	cout<< "---- after the expansion ----";

	for(int i = 0; i < numKeyWords; i++)
	{
		cout<<"Word " << dec << i <<"\n";
		keyWords[i].print();
	}

	/////////////////////////////////////////////////////////////////////////////////
	// THE REST OF THE FILE IS FOR TESTING PURPOSES ONLY
	/////////////////////////////////////////////////////////////////////////////////

	//populate with the temp matrix on page 199
	//note this temp matrix is defined in Matrices.h
	State testState; 
	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			testState.bytes[i][j] = Matrix_TestInput[i][j];
		}
	}

	cout << "---- ORIGINAL MATRIX ----\n";
	testState.print();

	ByteSub(testState);

	cout << "---- AFTER BYTE SUB ----\n";
	testState.print();
	
	ShiftRow(testState);

	cout << "---- AFTER SHIFT ROW ----\n";
	testState.print();

	MixColumn(testState);

	cout << "---- AFTER MIX COLUMN ----\n";
	testState.print();

	InvMixColumn(testState);

	cout << "---- AFTER INV MIX COLUMN --\n";
	testState.print();

	InvShiftRow(testState);

	cout << "---- AFTER INV SHIFT ROW ---\n";
	testState.print();

	InvByteSub(testState);

	cout << "---- AFTER INV BYTE SUB ----\n";
	testState.print();

	//testing purposes only
	Word tempWord;
	for(int i = 0; i < 4; i++)
	{
		tempWord.bytes[i] = Matrix_TestRijndael[i];
	}

	cout << "---- ORIGINAL WORD ----\n";
	tempWord.print();

	RotWord(tempWord);

	cout << "---- AFTER ROT WORD ----\n";
	tempWord.print();

	SubWord(tempWord);

	cout << "---- AFTER SUB WORD ----\n";
	tempWord.print();

	//attempt to calculate t using the RCon

	tempWord.bytes[0] ^= Matrix_RCon[1];

	cout << "---- AFTER THE XOR ATTEMPT ----";
	tempWord.print();

	return 0;
}

