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
// 04/27/2017 | DS | Removed array of key states - only going to use keyWords now.  Key Addition worked on.
// 04/28/2017 | DS | Placed much of this file into Cipher.cpp.  Code cleanup.
// 04/28/2017 | DS | Created MAX_MSG_SIZE and MAX_STATES for the array allocation issue.
// 04/28/2017 | DS | Fixed the array pass by reference issue.  New version of MixColumn.
// 04/29/2017 | DS | Put Cipher and Decrypt into a loop.  More debugging.
// 05/01/2017 | DS | Changed cipher output text file to ascii representation of the hex values.  

#include <iostream>			//for grabbing user input
#include <fstream>			//file i/o
#include <sstream>			//used for string manipulation
#include <string>			//for grabbing user input
#include <cstring>			//converts strings to 'c style' strings
#include <sys/stat.h> 		//for filesize	

//the external files
#include "ByteSub.h"		//for ByteSub() and InvByteSub()
#include "ShiftRow.h"		//for ShiftRow() and InvShiftRow()
#include "MixColumn.h"		//for MixColumn() and InvMixColumn()
#include "KeyAdd.h"			//for KeyAdd()
#include "State.h"			//for struct Block, State, Word
#include "Matrices.h" 		//for test matrices as well as S-boxes
#include "KeyExpansion.h"	//for RotWord and SubWord
#include "Cipher.h"

using namespace std;

#define BLOCK_SIZE_BIT 128
#define BLOCK_SIZE_CHAR 16

//max values for the array sizes
#define MAX_MSG_CHAR 3000
#define MAX_STATES 200
#define MAX_KEYWORDS 60

//TODO: accept keysize, filenames as arguments
int main()
{
	ifstream message, key;
	string messageName, keyName;
	int keySize = -1; //126, 192, or 256
	int numRounds = -1; //dependent on the keysize
	int numKeyWords = -1; //for the key expansion
	int groupSize = -1; //the number of 'words' in each group of key expansions

	printf("\nAES\n");
	printf("\nBy David Shimkus and Michael Caponi\n");

	//TODO: if arguments are passed in do not do this input/output printf stuff

	//check to see if we are encrypting our decrypting the input message
	int method = 0;
	bool done = false;
	while (!done)
	{
		printf("\nWhat do you want to do?\n");
		printf("1. Encrypt a text file.\n");
		printf("2. Decrypt a text file.\n");
		printf("Selection: ");
		cin >> method;
		if (method == 1 || method == 2)
		{
			done = true;
		}
	}

	//TODO: grab the PATH not just the filename (so can look at entire system not just local directory)
	if (method == 1)
	{
		//Encryption
		printf("\nPlease enter the plaintext filename: ");
	}
	else if (method == 2)
	{
		//Decryption
		printf("\nPlease enter the ciphertext filename: ");
	}
	else
	{
		//Should never print
		printf("\nPlease enter the message filename: ");
	}
	cin >> messageName;

	message.open(messageName.c_str());
	//TODO: file i/o error handling

	int selection;
	done = false;
	while (!done)
	{
		printf("\nWhat is the size of the key?\n");
		printf("1. 128 bit\n");
		printf("2. 192 bit\n");
		printf("3. 256 bit\n");
		printf("Selection: ");
		cin >> selection;

		switch (selection)
		{
		case  1:
		{
			keySize = 128;
			numRounds = 10;
			numKeyWords = 44;
			groupSize = 4;
			break;
		}
		case  2:
		{
			keySize = 192;
			numRounds = 12;
			numKeyWords = 52;
			groupSize = 6;
			break;
		}
		case  3:
		{
			keySize = 256;
			numRounds = 14;
			numKeyWords = 60;
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

	//find message length in CHARACTERS/BYTES
	int messageSizeChar = 0;
	string line;
	while (getline(message, line))
	{
		messageSizeChar += line.length() + 1;//add 1 for the newline character
	}

	//find out how many blocks we will need
	double numBlocksD = 0.0;
	int	numBlocks = 0;
	bool paddingNeeded = false;
	if(method == 1)
	{
		numBlocksD = (double)messageSizeChar / (double)BLOCK_SIZE_CHAR;
		numBlocks = messageSizeChar / BLOCK_SIZE_CHAR;
	}
	else if(method == 2)
	{
		//have to account for the two hex values
		numBlocksD = ( (double)messageSizeChar / 2 ) / BLOCK_SIZE_CHAR;
		numBlocks = ( (double)messageSizeChar / 2 ) / BLOCK_SIZE_CHAR;
	}
	else
	{
		//something went wrong...
	}

	if (numBlocksD > numBlocks)
	{
		//if there is a 'fraction' of a whole block we still need an extra block
		//TODO: eventually the unused bytes in this extra block will be populated with 0's
		numBlocks++;
		paddingNeeded = true;
	}

	//return to the beginning of the message stream
	message.clear();
	message.seekg(0, ios::beg);

	//actually grab the contents and place into a string
	stringstream buffer;
	buffer << message.rdbuf();
	string messageString = buffer.str();

	//copy the 'string' variable into a character array
	//char messageChars[messageSizeChar + 1];
	char messageChars[MAX_MSG_CHAR + 1];
	strncpy(messageChars, messageString.c_str(), sizeof(messageChars));
	messageChars[messageSizeChar + 1] = '\0'; //add this null guy to the end

	cout << "About to print messageChars: \n";
	printf("%s",messageChars);

	//split the message into block chunks
	block blocks[MAX_STATES];
	int counter = 0;
	for (int i = 0; i < numBlocks; i++)
	{
		if(method == 1)
		{
			counter = i * BLOCK_SIZE_CHAR;
			for (int j = 0; j < BLOCK_SIZE_CHAR; j++)
			{
				//pad with 0's
				if (i == numBlocks - 1 && paddingNeeded && counter >= messageSizeChar)
				{
					blocks[i].text[j] = '0';
				}
				else
				{
					blocks[i].text[j] = messageChars[counter];
				}

				counter++;
			}
		}
		else if(method == 2)
		{
			counter = i * BLOCK_SIZE_CHAR * 2;
			//char _2bytes[2];
			string _2bytes;
			for (int j = 0; j < BLOCK_SIZE_CHAR; j++)
			{
				//pad with 0's
				if (i == numBlocks - 1 && paddingNeeded && counter >= messageSizeChar)
				{
					//TODO: fix for decrypt
					//blocks[i].text[j] = '0';
					//cout << "mlaaaaaaaaaaaaaaaaaaaa\n";
				}
				else
				{
					//grab two ascii values at a time and combine them into one hex value
					//aka convert two bytes into one byte based on their ascii representation
					//BE CAREFUL WITH THIS
					_2bytes[0] = messageChars[counter];
					cout << "_2bytes[0]: " << _2bytes[0] << endl;
					counter++;
					_2bytes[1] = messageChars[counter];
					cout << "_2bytes[1]: " << _2bytes[1] << endl;

					string byteString = "";
					byteString += (_2bytes[0]);
					byteString += (_2bytes[1]);
					cout << "byteString: " << byteString << endl;

					//parse into an integer with base 16
					int byte = (int)strtol(byteString.c_str(), NULL, 16);
					cout << "byte: " << hex << byte << endl;

					/*
					//THIS IS PROBABLY THE EASIER WAY (?)
					//will have to grab the two hex values and combine them
					char byte1;
					char byte2;
					byte1 = blocks[i].text[blockTextIndex];
					blockTextIndex++;
					byte2 = blocks[i].text[blockTextIndex];
					
					//combine the two characters into a string
					stringstream ss;
					int finalByte;
					ss << hex << (int)byte1 << hex << (int)byte2;

					cout << "finalByte of state["<<i<<"]: " << ss.rdbuf() << endl;

					ss >> hex >> finalByte;

					states[i].bytes[j][k] = finalByte;
					*/

					//plug it into our block
					blocks[i].text[j] = byte;
				}
				counter++;
			}
		}
	}

	//OK so at this point the 'blocks' variable contains an array
	//of struct block's which contain the '.text' field that can
	//be passed to different portions of AES as appropriate
	//i.e. if we are going to pass the 3rd 'block' to AES it would
	//be the 3rd chunk of 128 bits i.e. 16 characters i.e. blocks[2].text
	//5/1 note: ^ this only applies to the encryption i.e. method==1

	//printf("%.*s\n", BLOCK_SIZE_CHAR, blocks[0].text);
	cout << "about to print blocks[0]...\n";
	printf("%.*s\n", BLOCK_SIZE_CHAR, blocks[0].text);

	//create an array of States out of the array of blocks
	State states[MAX_STATES];
	int blockTextIndex = 0;
	for (int i = 0; i < numBlocks; i++)
	{
		blockTextIndex = 0;
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				
				//can grab straight ascii
				//TODO: do the indeces need to be reversed?????????
				states[i].bytes[j][k] = blocks[i].text[blockTextIndex];
				//blockTextIndex++;
			
				//increment our iterator
				blockTextIndex++;
			}
		}
	}

	cout << "~~~~~~~~~~ AFTER POPULATION OF STATES ~~~~~~~~~\n";
	for(int i = 0; i < numBlocks; i++)
	{
		cout << "State #" << dec << i << endl;
		states[i].print();
		states[i].printAscii();
	}

	//now we need to get the input key - we will need it for key expansion
	const int keySizeChar = keySize / 8; //divide by 8 to convert bits to bytes

	//return to the beginning of the key stream
	key.clear();
	key.seekg(0, ios::beg);

	//actually grab the contents and place into a string
	stringstream buffer2;
	buffer2 << key.rdbuf();
	string keyString = buffer2.str();

	//copy the 'string' variable into a character array
	char keyChars[33];
	strncpy(keyChars, keyString.c_str(), sizeof(keyChars));

	//uncomment the following loop to populate with dummy data as per
	//pg 210 from the textbook (only works if 128 selected)
	
	for(int i = 0; i < keySizeChar; i++)
	{
		keyChars[i] = Matrix_TestKey[i];
		//keyChars[i] = 0x00;
	}
	

	keyChars[keySizeChar + 1] = '\0'; //add this null guy to the end

	//declare our array of keys that will be populated with the key expansions
	Word keyWords[MAX_KEYWORDS]; //the total "words" to be populated

	//begin key expansion
	//perform the initial population for keyWords[0] to keyWords[groupSize] 
	int offset = 0;
	for (int i = 0; i < groupSize; i++)
	{
		for (int j = offset; j < offset + 4; j++)
		{
			keyWords[i].bytes[j % 4] = keyChars[j];
		}
		offset += 4;
	}

	//compute the rest of the keyWords
	//TODO: modularize this (use groupsize and %'s) (maybe a function) !!
	if (keySize == 128)
	{
		Word tempWord;
		for (int i = 4; i < numKeyWords; i++)
		{
			if (i % 4 != 0)
			{
				for (int j = 0; j < 4; j++)
				{
					keyWords[i].bytes[j] = keyWords[i - 1].bytes[j] ^ keyWords[i - 4].bytes[j];
				}
			}
			else
			{
				//function g
				for (int k = 0; k < 4; k++)
				{
					tempWord.bytes[k] = keyWords[i - 1].bytes[k]; //initially populate the tempword		
				}
				//perform the rotation
				RotWord(tempWord);
				//apply the subword
				SubWord(tempWord);

				//XOR the [0]'th byte with RCon[i/4]
				tempWord.bytes[0] ^= Matrix_RCon[i / 4];

				//apply the addition (XOR?)of 't' aka tempWord
				for (int l = 0; l < 4; l++)
				{
					keyWords[i].bytes[l] = tempWord.bytes[l] ^ keyWords[i - 4].bytes[l];
				}
			}
		}
	}
	else if (keySize == 192)
	{
		//note: this is basically the same as 128 bit except for 6 word "groups" instead of 4
		Word tempWord;
		for (int i = 4; i < numKeyWords; i++)
		{
			if (i % 6 != 0)
			{
				for (int j = 0; j < 4; j++)
				{
					//note the keyWords[i-6] here instead of [i-4]
					keyWords[i].bytes[j] = keyWords[i - 1].bytes[j] ^ keyWords[i - 6].bytes[j];
				}
			}
			else
			{
				//function g
				for (int k = 0; k < 4; k++)
				{
					//note 'tempword' is the 't' variable from the book
					tempWord.bytes[k] = keyWords[i - 1].bytes[k]; //initially populate the tempword		
				}
				//perform the rotation
				RotWord(tempWord);
				//apply the subword
				SubWord(tempWord);

				//XOR the [0]'th byte it with Rconi/4
				tempWord.bytes[0] ^= Matrix_RCon[i / 4];

				//apply the addition (XOR?)of 't' aka tempWord
				for (int l = 0; l < 4; l++)
				{
					keyWords[i].bytes[l] = tempWord.bytes[l] ^ keyWords[i - 6].bytes[l];
				}
			}
		}
	}
	else if (keySize == 256)
	{
		Word tempWord;
		for (int i = 8; i < numKeyWords; i++)
		{
			if (i % 8 != 0)
			{
				//check for the additional step needed for 256
				//this is subsection 2.c. of pg 212 from the textbook
				if (i % 4 == 0)
				{
					for (int j = 0; j < 4; j++)
					{
						tempWord.bytes[j] = keyWords[i - 1].bytes[j];
					}
					//perform the SubWord
					SubWord(tempWord);

					for (int j = 0; j < 4; j++)
					{
						tempWord.bytes[j] ^= keyWords[i - 8].bytes[j];
					}
				}
				else
				{
					for (int j = 0; j < 4; j++)
					{
						keyWords[i].bytes[j] = keyWords[i - 1].bytes[j] ^ keyWords[i - 8].bytes[j];

					}
				}
			}
			else
			{
				for (int k = 0; k < 4; k++)
				{
					tempWord.bytes[k] = keyWords[i - 1].bytes[k]; //initially populate the tempword		
				}
				//perform the rotation
				RotWord(tempWord);
				//apply the subword
				SubWord(tempWord);
				//XOR it with Rcon[i/4]
				for (int k = 0; k < 4; k++)
				{
					tempWord.bytes[k] ^= Matrix_RCon[i / 4];
				}
				//apply the addition (XOR?)of 't' aka tempWord
				for (int l = 0; l < 4; l++)
				{
					keyWords[i].bytes[l] = tempWord.bytes[l] ^ keyWords[i - 8].bytes[l];
				}
			}
		}
	}
	else
	{
		//something went wrong...

	}

	//perform the actual encryption or decryption using all the pieces above
	if(method == 1)
	{
		//encryption
		for(int i = 0; i < numBlocks; i++)
		{
			for(int j = 0; j <= numRounds; j++)
			{
				//where the magic happens
				Cipher(states[i], keyWords, j, numRounds);
			}
		}

		cout << "~~~~~~~~~~ AFTER CIPHER OPERATION ON BLOCKS ~~~~~~~~~\n";
		for(int i = 0; i < numBlocks; i++)
		{
			cout << "State #" << dec << i << endl;
			states[i].print();
			states[i].printAscii();
		}
	}
	else if(method == 2)
	{
		cout << "########### INSIDE THE DECRYPTION BLOCK ############\n";
		for(int i = 0; i < numBlocks; i++)
		{
			cout << "State #" << dec << i << endl;
			states[i].print();
			states[i].printAscii();
		}

		//decryption
		for(int i = 0; i < numBlocks; i++)
		{
			for(int j = 0; j <= numRounds; j++)
			{
				//where the "un-magic" happens
				Decrypt(states[i], keyWords, j, numRounds);
			}
		}
	}
	else
	{
		//something went wrong...
	}

	//populate a buffer will all of the newly transformed state contents
	stringstream buffer3;
	for(int i = 0; i < numBlocks; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			for(int k = 0; k < 4; k++)
			{
				if(method == 1)
				{
					//have to write the hex values as ascii characters
					//get the ASCII value as well as its decimal value
					int byteTemp = states[i].bytes[j][k];

					//get the leftmost 4 bits
					byteTemp = (byteTemp >> 4) & ((1 << 4) - 1); //leftmost 4 bits
					int byte1 = byteTemp;

					//get the rightmost 4 bits
					byteTemp = states[i].bytes[j][k];
					byteTemp = (byteTemp >> 0) & ((1 << 4) - 1); //rightmost 4 bits
					int byte2 = byteTemp;

					//set the final byte
					int finalByte = 0;
					finalByte ^= byte1; //get the leftmost bits
					finalByte = finalByte << 4; //shift to leftmost places
					finalByte ^= byte2; //AND finalbyte with the rightmost bits

					//write both hex values (2 bytes) to the buffer
					buffer3 << setfill('0') << setw(2) << hex << finalByte;
				}
				else if(method == 2)
				{
					//can write straight ascii (in theory)
					buffer3 << (char)states[i].bytes[j][k];
				}
				else
				{
					//something went wrong...
				}
			}
		}
	}

	//write the transformed states to an output file
	string outputName;
	printf("\nPlease enter the output filename: ");
	cin >> outputName;
	ofstream output;
	output.open(outputName.c_str());
	output << buffer3.rdbuf();


	return 0;
}

