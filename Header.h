#pragma once
#ifndef HEADER
#define HEADER


#include <stdio.h>
#define NDEBUG
#include <assert.h>
#include "TXlib.h"
#include <stdlib.h>
#include <limits.h>

typedef long double element;
#define EL "%Lf"

#define ERRPTR (element*)0x0FF;

enum ErrorStatus
{
	STATUS_OK,
	NULL_PTR_STACK,
	NULL_PTR_DATA,
	CAPACITY_LESS_THAN_ZERO,
	SIZE_LESS_THAN_ZERO,
	CAPACITY_LESS_THAN_SIZE,
	STACK_CANARY_DAMAGED,
	DATA_CANARY_DAMAGED,
	STACK_OVERFLOW,
	HASH_DATA_MISMATCH,
	HASH_SIZE_MISMATCH,
	HASH_CAPACITY_MISMATCH,
};

enum frequency
{
	FREQUENCY_CHAR = 8,
	FREQUENCY_SHORT = 4,
	FREQUENCY_INT = 2,
	FREQUENCY_LONG = 1,
};

enum Constants
{
	c1 = 0xcc9e2d51,
	c2 = 0x1b873593,
	n = 0xe6546b64,
	m = 0x5,
	r1 = 15,
	r2 = 13,
	final_shift_1 = 16,
	final_shift_2 = 13,
	final_c3 = 0x85ebca6b,
	final_c4 = 0xc2b2ae35,
};


struct Hash
{
	unsigned long long hashData;
	unsigned long long hashSize;
	unsigned long long hashCapacity;
};


struct Stack
{
	long long upCanary1;
	long long upCanary2;
	element* data;
	long long size;
	long long capañity;
	ErrorStatus status;
	Hash hash;
	long long downCanary1;
	long long downCanary2;
};



//---------------------------------------------------------------------------------------------------------------------------------
//! The function allocates understand for a stack of size "capacity", initializes the stack with empty contents and puts canaries
//! 
//! @param [in] stack			pointer to type Stack
//! @param [in] capasity		Initialized stack capacity
//!
//---------------------------------------------------------------------------------------------------------------------------------
void StackCtor(Stack* stack, long long capacity);


//---------------------------------------------------------------------------------------------------------------------------------
//! The function puts the requested element into the first free cell of the stack
//! 
//! @param [in] stack			pointer to type Stack
//! @param [in] element			the data object to be pushed onto the stack. Always follows the previously placed element
//!
//---------------------------------------------------------------------------------------------------------------------------------
void StackPush(Stack* stack, element value);


//---------------------------------------------------------------------------------------------------------------------------------
//! The function allocates additional space if the stack has run out. Called inside the StackPush function
//! 
//! @param [in] stack			pointer to type Stack
//! @param [in] new_capacity		New stack capacity. Should be larger than the previous one
//!
//---------------------------------------------------------------------------------------------------------------------------------
void StackResize(Stack* stack, long long new_capacity);

//---------------------------------------------------------------------------------------------------------------------------------
//! The function returns the last element on the stack
//! 
//! @param [in] stack			pointer to type Stack
//! @param [in] value			pounter to last item on the stack
//!
//! @param [out] *value			the last element on the stack
//---------------------------------------------------------------------------------------------------------------------------------
void StackPop(Stack* stack, element* value);


//---------------------------------------------------------------------------------------------------------------------------------
//! The function frees the memory allocated for the stack
//! 
//! @param [in] stack			pointer to type Stack
//!
//---------------------------------------------------------------------------------------------------------------------------------
void StackDtor(Stack* stack);


//---------------------------------------------------------------------------------------------------------------------------------
//! The function provides detailed information about the stack as well as any errors that may have occurred
//! 
//! @param [in] stack			pointer to type Stack
//! @param [in] errmessage		error information
//!
//---------------------------------------------------------------------------------------------------------------------------------
void StackDump(const Stack* stack, const char* errmessage);


//---------------------------------------------------------------------------------------------------------------------------------
//! The function checks the stack for the presence of an error
//! 
//! @param [in] stack			pointer to type Stack
//!
//! @return true if there are no errors, otherwise false
//---------------------------------------------------------------------------------------------------------------------------------
bool StackOk(Stack* stack);


//---------------------------------------------------------------------------------------------------------------------------------
//! The function creates a 64 bit hash for the stack
//! 
//! @param [in] data			array with stack elements
//! @param [in] capacity		stack capacity	
//! @param [in] seed			initial value
//!
//! @return 64 bit hash
//---------------------------------------------------------------------------------------------------------------------------------
unsigned long long StackHash(element* data, long long capacity, int seed);

//---------------------------------------------------------------------------------------------------------------------------------
//! The function creates a 64 bit hash for some value
//! 
//! @param [in] value			some value	
//! @param [in] seed			initial value
//!
//! @return 64 bit hash
//---------------------------------------------------------------------------------------------------------------------------------
unsigned long long ValueHash(long long value, int seed);


//---------------------------------------------------------------------------------------------------------------------------------
//! Hash helper function that splits an array into 8-bit words
//! 
//! @param [in] data			pointer to array	
//! @param [in] nBlock			number of block (words)
//!
//! @return 8-bit words
//---------------------------------------------------------------------------------------------------------------------------------
unsigned long long CreateBlock8(element* data, size_t nBlock);


//---------------------------------------------------------------------------------------------------------------------------------
//! Hash helper function that splits an array into 16-bit words
//! 
//! @param [in] data			pointer to array	
//! @param [in] nBlock			number of block (words)
//!
//! @return 16-bit words
//---------------------------------------------------------------------------------------------------------------------------------
unsigned long long CreateBlock16(element* data, size_t nBlock);


//---------------------------------------------------------------------------------------------------------------------------------
//! Hash helper function that splits an array into 32-bit words
//! 
//! @param [in] data			pointer to array	
//! @param [in] nBlock			number of block (words)
//!
//! @return 32-bit words
//---------------------------------------------------------------------------------------------------------------------------------
unsigned long long CreateBlock32(element* data, size_t nBlock);


//---------------------------------------------------------------------------------------------------------------------------------
//! Hash helper function that splits an array into 64-bit words
//! 
//! @param [in] data			pointer to array	
//! @param [in] nBlock			number of block (words)
//!
//! @return 64-bit words
//---------------------------------------------------------------------------------------------------------------------------------
unsigned long long CreateBlock64(element* data, size_t nBlock);


//---------------------------------------------------------------------------------------------------------------------------------
//! The function calculates the hash for an object of size 8 bits
//! 
//! @param [in] data			pointer to array	
//! @param [in] size			size of array
//! @param [in] hash			pointer to hash
//!
//! @param [out] *hash			hash
//!
//! @return 8-bit words
//---------------------------------------------------------------------------------------------------------------------------------
void Hash8bit(element* data, size_t size, unsigned long long* hash);


//---------------------------------------------------------------------------------------------------------------------------------
//! The function calculates the hash for an object of size 16 bits
//! 
//! @param [in] data			pointer to array	
//! @param [in] size			size of array
//! @param [in] hash			pointer to hash
//!
//! @param [out] *hash			hash
//!
//! @return 8-bit words
//---------------------------------------------------------------------------------------------------------------------------------
void Hash16bit(element* data, size_t size, unsigned long long* hash);


//---------------------------------------------------------------------------------------------------------------------------------
//! The function calculates the hash for an object of size 32 bits
//! 
//! @param [in] data			pointer to array	
//! @param [in] size			size of array
//! @param [in] hash			pointer to hash
//!
//! @param [out] *hash			hash
//!
//! @return 8-bit words
//---------------------------------------------------------------------------------------------------------------------------------
void Hash32bit(element* data, size_t size, unsigned long long* hash);


//---------------------------------------------------------------------------------------------------------------------------------
//! The function calculates the hash for an object of size 64 bits
//! 
//! @param [in] data			pointer to array	
//! @param [in] size			size of array
//! @param [in] hash			pointer to hash
//! 
//! @param [out] *hash			hash
//!
//! @return 8-bit words
//---------------------------------------------------------------------------------------------------------------------------------
void Hash64bit(element* data, size_t size, unsigned long long* hash);


//---------------------------------------------------------------------------------------------------------------------------------
//! The function rotates the bits of a 64-bit data type to the left
//! 
//! @param [in] object			The object, the cyclic permutation of the bits of which needs to be done	
//! @param [in] n			How many bits to rotate
//!
//! @return Bit-swapped object
//---------------------------------------------------------------------------------------------------------------------------------
unsigned long long RotateLeft(unsigned long long object, int n);


//---------------------------------------------------------------------------------------------------------------------------------
//! The function updates the hash of the stack
//! 
//! @param [in] stack			pointer to type Stack
//!
//---------------------------------------------------------------------------------------------------------------------------------
void UpdateHash(Stack* stack);


#define CHECKSTACK {							    \
	if (StackOk(stack) == false)					     \
	{								      \
		StackDump(stack, __FUNCSIG__);				       \
		puts("An error has occurred. We will fix it soon");		\
		exit(EXIT_FAILURE);						 \
	}						       		          \
}

#define StkData(data ,i) (*(element*)((char*)(data) + 2 * sizeof(long long) + (i) * sizeof(*data)))

#endif // !HEADER

