#include "Header.h"



void StackCtor(Stack* stack, long long capaсity)
{
	assert(stack);
	if (capaсity < 0)
	{
		capaсity = 0;
	}

	stack->data = (element*)malloc(capaсity * sizeof(*stack->data) + 4 * sizeof(long long));
	for (int i = 0; i < capaсity * sizeof(*stack->data) + 4 * sizeof(long long); i++)
	{
		*((char*)stack->data + i) = 0;
	}

	*(long long*)((char*)stack->data + 0) = 0xFA11ED;
	*(long long*)((char*)stack->data + sizeof(long long)) = 0xFA11ED;
	*(long long*)((char*)stack->data + 2 * sizeof(long long) + capaсity * sizeof(*stack->data)) = 0xFA11ED;
	*(long long*)((char*)stack->data + 2 * sizeof(long long) + capaсity * sizeof(*stack->data) + sizeof(long long)) = 0xFA11ED;

	stack->capaсity = capaсity;
	stack->size = 0;
	stack->status = STATUS_OK;
	stack->upCanary1 = 0xFA11ED;
	stack->upCanary2 = 0xFA11ED;
	stack->downCanary1 = 0xFA11ED;
	stack->downCanary2 = 0xFA11ED;

	UpdateHash(stack);
	CHECKSTACK;
}


void StackPush(Stack* stack, element value)
{
	CHECKSTACK;

	if (stack->size < stack->capaсity)
	{
		StkData(stack->data, stack->size) = value;
		stack->size++;
		UpdateHash(stack);
	}
	else
	{
		StackResize(stack, stack->capaсity * 2);
		StkData(stack->data, stack->size) = value;
		stack->size++;
		UpdateHash(stack);
	}

	CHECKSTACK;
}


void StackResize(Stack* stack, long long new_capaсity)
{
	CHECKSTACK;

	if (new_capaсity <= stack->capaсity)
	{
		stack->status = STACK_OVERFLOW;
		puts("An error has occurred. Your device has run out of free memory");
		StackDump(stack, "YOUR DEVICE HAS RUN OUT OF FREE MEMORY. THE PROGRAM COULD NOT ALLOCATE MEMORY TO INCREASE THE STACK SIZE");
		exit(EXIT_FAILURE);
	}
	else
	{
		element* dataResize = (element*)malloc(new_capaсity * sizeof(*stack->data) + 4 * sizeof(long long));
		if (dataResize == NULL)
			StackResize(stack, new_capaсity * 8 / 10);
		else
		{
			for (int i = 0; i < new_capaсity * sizeof(*stack->data) + 4 * sizeof(long long); i++)
			{
				*((char*)dataResize + i) = 0;
			}

			*(long long*)((char*)dataResize + 0) = 0xFA11ED;
			*(long long*)((char*)dataResize + sizeof(long long)) = 0xFA11ED;

			for (int i = 0; i < stack->capaсity; i++)
			{
				StkData(dataResize, i) = StkData(stack->data, i);
			}

			*(long long*)((char*)dataResize + 2 * sizeof(long long) + new_capaсity * sizeof(*stack->data)) = 0xFA11ED;
			*(long long*)((char*)dataResize + 2 * sizeof(long long) + new_capaсity * sizeof(*stack->data) + sizeof(long long)) = 0xFA11ED;

			free(stack->data);
			stack->data = dataResize;
			stack->capaсity = new_capaсity;
			UpdateHash(stack);
		}
	}
	CHECKSTACK;
}


void StackPop(Stack* stack, element* value)
{
	CHECKSTACK;
	if (stack->size > 0)
	{
		*value = StkData(stack->data, --stack->size);
		UpdateHash(stack);
	}
	else
	{
		*value = 0;
		StackDump(stack, "Incorrect use of the stack. Trying to use StackPop function for stack size = 0");
	}
	CHECKSTACK;
}


void StackDtor(Stack* stack)
{
	CHECKSTACK;
	free(stack->data);
	stack->data = ERRPTR;
}


bool StackOk(Stack* stack)
{
	assert(stack);

	bool status = true;

	if (stack->data == NULL)
	{
		stack->status = NULL_PTR_DATA;
		status = false;
	}
	if (stack->capaсity < 0)
	{
		stack->status = CAPACITY_LESS_THAN_SIZE;
		status = false;
	}
	if (stack->size < 0)
	{
		stack->status = SIZE_LESS_THAN_ZERO;
		status = false;
	}
	if (stack->size > stack->capaсity)
	{
		stack->status = CAPACITY_LESS_THAN_ZERO;
		status = false;
	}
	if (stack->upCanary1 != 0xFA11ED)
	{
		stack->status = STACK_CANARY_DAMAGED;
		status = false;
	}
	if (stack->upCanary2 != 0xFA11ED)
	{
		stack->status = STACK_CANARY_DAMAGED;
		status = false;
	}
	if (stack->downCanary1 != 0xFA11ED)
	{
		stack->status = STACK_CANARY_DAMAGED;
		status = false;
	}
	if (stack->downCanary2 != 0xFA11ED)
	{
		stack->status = STACK_CANARY_DAMAGED;
		status = false;
	}
	if (*(long long*)((char*)stack->data + 0) != 0xFA11ED)
	{
		stack->status = DATA_CANARY_DAMAGED;
		status = false;
	}
	if (*(long long*)((char*)stack->data + sizeof(long long)) != 0xFA11ED)
	{
		stack->status = DATA_CANARY_DAMAGED;
		status = false;
	}
	if (*(long long*)((char*)stack->data + 2 * sizeof(long long) + stack->capaсity * sizeof(*stack->data)) != 0xFA11ED)
	{
		stack->status = DATA_CANARY_DAMAGED;
		status = false;
	}
	if (*(long long*)((char*)stack->data + 2 * sizeof(long long) + stack->capaсity * sizeof(*stack->data) + sizeof(long long)) != 0xFA11ED)
	{
		stack->status = DATA_CANARY_DAMAGED;
		status = false;
	}
	if (stack->hash.hashCapacity != ValueHash(stack->capaсity, 1))
	{
		stack->status = HASH_CAPACITY_MISMATCH;
		status = false;
	}
	if (stack->hash.hashSize != ValueHash(stack->size, 1))
	{
		stack->status = HASH_SIZE_MISMATCH;
		status = false;
	}
	if (stack->hash.hashData != StackHash(stack->data, stack->capaсity, 1))
	{
		stack->status = HASH_DATA_MISMATCH;
		status = false;
	}
	return status;
}


void StackDump(const Stack* stack, const char* errmessage)
{
	FILE* errfile = NULL;
	fopen_s(&errfile, "error.txt", "w");
	if (errfile == NULL)
		errfile = stdout;
	fprintf(errfile, "ERROR: %s\n", errmessage);
	if (stack->status == HASH_DATA_MISMATCH)
		fprintf(errfile, "Error: Hash Data mismatch\n");
	if (stack->status == HASH_SIZE_MISMATCH)
		fprintf(errfile, "Error: Hash size mismatch\n");
	if (stack->status == HASH_CAPACITY_MISMATCH)
		fprintf(errfile, "Error: Hash capacity mismatch\n");
	fprintf(errfile, "Stack [%p] (error number %d)\n\t{\n", stack, stack->status);
	fprintf(errfile, "\t Canary 1 = %llX\n", stack->upCanary1);
	fprintf(errfile, "\t Canary 2 = %llX\n", stack->upCanary2);
	fprintf(errfile, "\t size = %lld\n", stack->size);
	fprintf(errfile, "\t capacity = %lld\n", stack->capaсity);
	fprintf(errfile, "\t data [%p]\n\t\t{\n", stack->data);
	fprintf(errfile, "\t\t C[0] = %llX\n", *(long long*)((char*)stack->data + 0));
	fprintf(errfile, "\t\t C[1] = %llX\n", *(long long*)((char*)stack->data + sizeof(long long)));

	for (long long i = 0; i < stack->size; i++)
	{
		fprintf(errfile, "\t\t *[%lld] = " EL "\n", i + 2, StkData(stack->data, i));
	}
	for (long long i = stack->size; i <= stack->capaсity; i++)
	{
		fprintf(errfile, "\t\t  [%lld] = " EL "\n", i + 2, StkData(stack->data, i));
	}

	fprintf(errfile, "\t\t C[%lld] = %llX\n", stack->capaсity + 2, *(long long*)((char*)stack->data + 2 * sizeof(long long) + stack->capaсity * sizeof(*stack->data)));
	fprintf(errfile, "\t\t C[%lld] = %llX\n\t\t}\n", stack->capaсity + 3, *(long long*)((char*)stack->data + 2 * sizeof(long long) + stack->capaсity * sizeof(*stack->data) + sizeof(long long)));
	fprintf(errfile, "\t Canary 3 = %llX\n", stack->downCanary1);
	fprintf(errfile, "\t Canary 4 = %llX\n\t}\n", stack->downCanary2);
}
