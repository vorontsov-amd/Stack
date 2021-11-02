#pragma once
#ifndef HEADER
#define HEADER


#include <stdio.h>
//#define NDEBUG 1
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
	ÑAPACITY_LESS_THAN_ZERO,
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
	size_t size;
	size_t capañity;
	ErrorStatus status;
	Hash hash;
	long long downCanary1;
	long long downCanary2;
};

void StackCtor(Stack* stack, size_t capasity);
void StackPush(Stack* stack, element value);
void StackResize(Stack* stack, size_t new_capasity);
void StackPop(Stack* stack, element* value);
void StackDtor(Stack* stack);
void StackDump(const Stack* stack, const char* errmessage);
bool StackOk(Stack* stack);
unsigned long long MurMurHash(element* data, size_t capacity, int seed);
unsigned long long SimpleHash(size_t value, int seed);
unsigned long long CreateBlock8(element* data, size_t nBlock);
unsigned long long CreateBlock16(element* data, size_t nBlock);
unsigned long long CreateBlock32(element* data, size_t nBlock);
unsigned long long CreateBlock64(element* data, size_t nBlock);
void Hash8bit(element* data, size_t size, unsigned long long* hash);
void Hash16bit(element* data, size_t size, unsigned long long* hash);
void Hash32bit(element* data, size_t size, unsigned long long* hash);
void Hash64bit(element* data, size_t size, unsigned long long* hash);
unsigned long long RotateLeft(unsigned long long k, int n);
void UpdateHash(Stack* stack);


#define CHECKSTACK {									    \
	if (StackOk(stack) == false)					         \
	{													      \
		StackDump(stack, __FUNCSIG__);						   \
		puts("An error has occurred. We will fix it soon");		\
		exit(EXIT_FAILURE);										 \
	}														      \
}

#define StkData(i) (*(element*)((char*)stack->data + 2 * sizeof(long long) + (i) * sizeof(*stack->data)))

#endif // !HEADER

