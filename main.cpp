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

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <cstring>
#include <sys/stat.h> //for filesize
#include <vector>
#include <stdlib.h>
#include <stdio.h>

//the external files
#include "ByteSub.h"
#include "ShiftRow.h"
#include "MixColumn.h"
#include "KeyAdd.h"
#include "State.h"

using namespace std;

#define BLOCK_SIZE_BIT 128
#define BLOCK_SIZE_CHAR 16   //a single character is stored as a byte, so 16 bytes = 128 bits
#define MAX_MSG_LENGTH 10000 //TODO: let program handle MUCH larger messages

//ended up storing everything in this struct for the vector implementation
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
	int keySize = -1;

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
		case  1: keySize = 128; break;
		case  2: keySize = 192; break;
		case  3: keySize = 256; break;
		default: keySize = -1;
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
	//struct stat st;
	//int messageSizeChar = stat(messageName.c_str(), &st);
	int messageSizeChar = 0;
	string line;
	while(getline(message, line))
	{
		messageSizeChar += line.length() + 1;//add 1 for the newline character
	}
	
	//cout << "messageSizeChar: " << messageSizeChar << "\n";//works.

	//TODO:
	//int messageSizeBits = messageSizeChar * 8; //multiply by 8 to convert bytes to bits

	//find out how many blocks we will need
	double	numBlocksD = (double)messageSizeChar / (double)BLOCK_SIZE_CHAR;
	int		numBlocks  = messageSizeChar / BLOCK_SIZE_CHAR;
	//cout << "numBlocksD: " << numBlocksD << "\n";
	//cout << "numBlocks : " << numBlocks << "\n";
	if (numBlocksD > numBlocks)
	{
		//if there is a 'fraction' of a whole block we still need an extra block
		numBlocks++;
	}

	//cout << "Num blocks: " << numBlocks << "\n\n";

	//return to the beginning of the message stream
	message.clear();
	message.seekg(0, ios::beg);

	//actually grab the contents and place into a string
	stringstream buffer;
	buffer << message.rdbuf();
	string messageString = buffer.str();

	//printf(messageString.c_str());

	cout << "messageString.length(): " << messageString.length() << "\n\n";

	//copy the 'string' variable into a character array
	char messageChars[messageSizeChar + 1];
	strncpy(messageChars, messageString.c_str(), sizeof(messageChars));
	messageChars[messageSizeChar + 1] = '\0'; //add this null guy to the end

	//printf(messageChars);

	cout << "\n\n";

	//uncomment out the following to view the entire input file as char array
	//for(int i = 0; i < messageSizeChar; i++)
	//{
	//	printf("\nmessageChars[%i]: %c", i, messageChars[i]);
	//}

	cout << "\n\n";

	//printf(messageChars);

	//split the message into chunks
	block blocks[numBlocks];
	int counter = 0;
	for(int i = 0; i < numBlocks; i++)
	{
		counter = i * BLOCK_SIZE_CHAR;
		for(int j = 0; j < BLOCK_SIZE_CHAR; j++)
		{
			//char c = new char((char)messageChars[counter]);
			//blocks[i].text[j] = c;
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

	//convert ALL characters to their hex representation
	/*
	char a = 'Z';

	cout << "ASCII: " << a << endl;

	int temp = a;
	cout << "Decimal: " << temp << endl;

	stringstream ss;
	ss << hex << temp; // int decimal_value
	//ss << hex << messageString; //for the entire input string
	
	string res ( ss.str() );

	cout << "HEX: " << res << endl;
	*/


	//cout << temp;

	//TODO: actually implement AES LOL


	//for each block
	//create initial state from the input block
	//perform initial key addition -> OUTPUT A STATE

	//ROUNDS

	//BYTE SUB
	//take that state in as input
	//state outputBS = ByteSub(outputOfInitialKeyAddition);

	State tempState;

	int k = 0;
	int l = 0;

	for(int i = 0; i < 4; i++)
	{
		for(int j = 0; j < 4; j++)
		{
			
			//TODO: make sure the elements in the state's matrix don't get populated vertically(?)

			tempState.bytes[j][i] = blocks[k].text[l];

			l++;

		}
	}

	//create a new state object to receive the new data from ByteSub
	//TODO: perform this operation in-place in memory and don't create new objects (?)
	State tempState2 = ByteSub(tempState);

	cout << "\n!!!!!!!!!!!!!!!!!!! " << tempState2.ByteSub_values[0][0] << " !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n";

	//INVERSE BYTE SUB
	//State tempState3 = InvByteSub(tempState2);
	//cout <<"\n!!!!!!!!!!!!!!!!!!!!!!!!! " << tempState3.ByteSub_values[0][0] << "!!!!!!!!!!!!!!!!!!!!!!!!!!!!!\n\n";

	State tempState4 = ShiftRow(tempState2);

	cout << "\n!!!!!!!!!!!!!!!!!!! " << tempState4.bytes[0][0] << " !!!!!!!!!!!!!!!!!!!!!\n";


	//SHIFT ROW
	//take 

	//MIX COLUMN

	//KEY ADDITION



	



	return 0;
}

