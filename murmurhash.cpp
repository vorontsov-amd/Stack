#include "Header.h"

#define data(i, frequency, shift) ((*(unsigned long long*)((char*)data + 2 * sizeof(long long) + (nBlock * (frequency) + (i)) * sizeof(*data)) | 0LL) << (shift))



unsigned long long StackHash(element* data, long long capacity, int seed)
{
	unsigned long long hash = seed;

	switch (sizeof(*data))
	{
	case 1: Hash8bit(data, capacity,  &hash); break;
	case 2: Hash16bit(data, capacity, &hash); break;
	case 4: Hash32bit(data, capacity, &hash); break;
	case 8: Hash64bit(data, capacity, &hash); break;
	default: Hash64bit(data, capacity, &hash); break;
	}
	return hash;
}

unsigned long long RotateLeft(unsigned long long object, int n)
{
	unsigned long long temp = (object >> (sizeof(object) * CHAR_BIT - n));
	object = object << n;
	return temp | object;
}

unsigned long long CreateBlock8(char* data, size_t nBlock)
{
	return (data(0, FREQUENCY_CHAR, 56)) + (data(1, FREQUENCY_CHAR, 48)) + (data(2, FREQUENCY_CHAR, 40)) + (data(3, FREQUENCY_CHAR, 32)) +
	   	   (data(4, FREQUENCY_CHAR, 24)) + (data(5, FREQUENCY_CHAR, 16)) + (data(6, FREQUENCY_CHAR, 8))  + (data(7, FREQUENCY_CHAR, 0));
}

unsigned long long CreateBlock16(short* data, size_t nBlock)
{
	return (data(0, FREQUENCY_SHORT, 48)) + (data(1, FREQUENCY_SHORT, 32)) + (data(2, FREQUENCY_SHORT, 16)) + (data(3, FREQUENCY_SHORT, 0));
}

unsigned long long CreateBlock32(int* data, size_t nBlock)
{
	return (data(0, FREQUENCY_INT, 32)) + (data(1, FREQUENCY_INT, 0));
}

unsigned long long CreateBlock64(long long* data, size_t nBlock)
{
	return (data(0, FREQUENCY_LONG, 0));
}



#undef data
#define data(i, frequency, shift) ((*(unsigned long long*)((char*)data + 2 * sizeof(long long) + (number64blocks * (frequency) + (i)) * sizeof(*data)) | 0LL) << (shift))



void Hash8bit(element* data, size_t size, unsigned long long* hash)
{	
	size_t remaining_bytes = (size * sizeof(*data)) % 8;
	size_t number64blocks = (size * sizeof(*data) - remaining_bytes) / 8;

	for (size_t nBlock = 0; nBlock < number64blocks; nBlock++)
	{
		unsigned long long k = CreateBlock8((char*)(data), nBlock);
		k *= c1;
		k = RotateLeft(k, r1);
		k *= c2;
		*hash ^= k;
		*hash = RotateLeft(*hash, r2);
		*hash = *hash * m + n;
	}

	unsigned long long k = 0;
	switch (remaining_bytes)
	{
	case 7: k = (data(0, FREQUENCY_CHAR, 48)) + (data(1, FREQUENCY_CHAR, 40)) + (data(2, FREQUENCY_CHAR, 32)) +
				(data(3, FREQUENCY_CHAR, 24)) + (data(4, FREQUENCY_CHAR, 16)) + (data(5, FREQUENCY_CHAR, 8)) +
				(data(6, FREQUENCY_CHAR, 0));
		break;
	case 6: k = (data(0, FREQUENCY_CHAR, 40)) + (data(1, FREQUENCY_CHAR, 32)) + (data(2, FREQUENCY_CHAR, 24)) +
				(data(3, FREQUENCY_CHAR, 16)) + (data(4, FREQUENCY_CHAR, 8))  + (data(5, FREQUENCY_CHAR, 0));
		break;
	case 5: k = (data(0, FREQUENCY_CHAR, 32)) + (data(1, FREQUENCY_CHAR, 24)) + (data(2, FREQUENCY_CHAR, 16)) +
				(data(3, FREQUENCY_CHAR, 8))  + (data(4, FREQUENCY_CHAR, 0));
		break;
	case 4: k = (data(0, FREQUENCY_CHAR, 24)) + (data(1, FREQUENCY_CHAR, 16)) + (data(2, FREQUENCY_CHAR, 8)) +
				(data(3, FREQUENCY_CHAR, 0));
		break;
	case 3: k = (data(0, FREQUENCY_CHAR, 16)) + (data(1, FREQUENCY_CHAR, 8))  + (data(2, FREQUENCY_CHAR, 0));
		break;
	case 2: k = (data(0, FREQUENCY_CHAR, 8))  + (data(1, FREQUENCY_CHAR, 0));
		break;
	case 1: k = (data(0, FREQUENCY_CHAR, 0));
		break;
	default:
		break;
	}

	k *= c1;
	k = RotateLeft(k, r1);
	k *= c2;
	*hash ^= k;
	*hash ^= (size * sizeof(*data));

	*hash ^= (*hash >> final_shift_1);
	*hash *= final_c3;
	*hash ^= (*hash >> final_shift_2);
	*hash *= final_c4;
	*hash *= (*hash >> final_shift_1);
}

void Hash16bit(element* data, size_t size, unsigned long long* hash)
{
	size_t remaining_bytes = (size * sizeof(*data)) % 4;
	size_t number64blocks = (size * sizeof(*data) - remaining_bytes) / 4;

	for (size_t nBlock = 0; nBlock < number64blocks; nBlock++)
	{
		unsigned long long k = CreateBlock16((short*)(data), nBlock);
		k *= c1;
		k = RotateLeft(k, r1);
		k *= c2;
		*hash ^= k;
		*hash = RotateLeft(*hash, r2);
		*hash = *hash * m + n;
	}

	unsigned long long k = 0;
	switch (remaining_bytes)
	{
	case 3: k = (data(0, FREQUENCY_SHORT, 32)) + (data(1, FREQUENCY_SHORT, 16)) + (data(2, FREQUENCY_SHORT, 0));
		break;
	case 2: k = (data(0, FREQUENCY_SHORT, 16)) + (data(1, FREQUENCY_SHORT, 0));
		break;
	case 1: k = (data(0, FREQUENCY_SHORT, 0));
		break;
	default:
		break;
	}

	k *= c1;
	k = RotateLeft(k, r1);
	k *= c2;
	*hash ^= k;
	*hash ^= (size * sizeof(*data));

	*hash ^= (*hash >> final_shift_1);
	*hash *= final_c3;
	*hash ^= (*hash >> final_shift_2);
	*hash *= final_c4;
	*hash *= (*hash >> final_shift_1);
}

void Hash32bit(element* data, size_t size, unsigned long long* hash)
{
	size_t remaining_bytes = (size * sizeof(*data)) % 2;
	size_t number64blocks = (size * sizeof(*data) - remaining_bytes) / 2;

	for (size_t nBlock = 0; nBlock < number64blocks; nBlock++)
	{
		unsigned long long k = CreateBlock32((int*)(data), nBlock);
		k *= c1;
		k = RotateLeft(k, r1);
		k *= c2;
		*hash ^= k;
		*hash = RotateLeft(*hash, r2);
		*hash = *hash * m + n;
	}

	unsigned long long k = 0;
	switch (remaining_bytes)
	{
	case 1: k = (data(0, FREQUENCY_LONG, 0));
		break;
	default:
		break;
	}

	k *= c1;
	k = RotateLeft(k, r1);
	k *= c2;
	*hash ^= k;
	*hash ^= (size * sizeof(*data));

	*hash ^= (*hash >> final_shift_1);
	*hash *= final_c3;
	*hash ^= (*hash >> final_shift_2);
	*hash *= final_c4;
	*hash *= (*hash >> final_shift_1);
}

void Hash64bit(element* data, size_t size, unsigned long long* hash)
{
	size_t number64blocks = size * sizeof(*data);

	for (size_t nBlock = 0; nBlock < number64blocks; nBlock++)
	{
		unsigned long long k = CreateBlock64((long long*)(data), nBlock);
		k *= c1;
		k = RotateLeft(k, r1);
		k *= c2;
		*hash ^= k;
		*hash = RotateLeft(*hash, r2);
		*hash = *hash * m + n;
	}

	unsigned long long k = 0;
	k *= c1;
	k = RotateLeft(k, r1);
	k *= c2;
	*hash ^= k;
	*hash ^= (size * sizeof(*data));

	*hash ^= (*hash >> final_shift_1);
	*hash *= final_c3;
	*hash ^= (*hash >> final_shift_2);
	*hash *= final_c4;
	*hash *= (*hash >> final_shift_1);
}


unsigned long long ValueHash(long long value, int seed)
{
	unsigned long long hash = seed;
	
	unsigned long long k = value;
	k *= c1;
	k = RotateLeft(k, r1);
	k *= c2;
	hash ^= k;
	hash = RotateLeft(hash, r2);
	hash = hash * m + n;
	hash ^= k;
	hash ^= (sizeof(value));

	hash ^= (hash >> final_shift_1);
	hash *= final_c3;
	hash ^= (hash >> final_shift_2);
	hash *= final_c4;
	hash *= (hash >> final_shift_1);

	return hash;
}

void UpdateHash(Stack* stack)
{
	stack->hash.hashCapacity = ValueHash(stack->capaсity, 1);
	stack->hash.hashSize = ValueHash(stack->size, 1);
	stack->hash.hashData = StackHash(stack->data, stack->capaсity, 1);
}
