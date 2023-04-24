# Stack

Stack is a C implementation of a stack data structure. It provides a simple and efficient way to store and access data in a Last-In-First-Out (LIFO) fashion.

## Features

- Supports all standard stack operations (push, pop)
- In case of data corruption gives dump file with contents and error name
- Uses canaries to protect data
- Uses MurMurHash to protect data (every time data is accessed, hash is recalculated and compared)

## Usage

### Compilation and Instalation

To use the stack, compile your files along with my .cpp files

### Examples

Here's an example program that demonstrates how to use the stack library:

```c
#include <stdio.h>
#include "Stack.h"

int main() {
    Stack stack;
    StackCtor(&stack);

    StackPush(&stack, 1);
    StackPush(&stack, 2);
    StackPush(&stack, 3);

    int value = StackPop(&stack);
    printf("%d\n", value);

    StackDtor(&stack);

    return 0;
}
```
```
g++ main.cpp Stack.cpp murmurhash.cpp -o main
```

## License

This project is licensed under the MIT License. See the LICENSE file for details.

## Contributing

If you want to contribute to the project, feel free to submit a pull request.