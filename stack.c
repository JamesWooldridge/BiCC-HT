#include <stdio.h>
#include <stdlib.h>
#include "stack.h" 

/**
 *	Initialises a new, empty, stack of maxSize size
 */
Stack * initStack(int maxSize) {
	Stack *stack = (Stack *) malloc(sizeof(Stack));						// Allocate memory for the Stack
	stack->contents = malloc(sizeof(StackElement) * maxSize);			// Allocate a new array to hold the contents

	if(stack->contents == NULL) {
	  	fprintf(stderr, "Insufficient memory to init stack.\n");
	   	exit(1);
	}

	stack->maxSize = maxSize;
	stack->top = -1;

	return stack;
}

/**
 *	Frees all memory associated with the stack. Stack cannot
 *	be used again until reinitialised
 */
void destroyStack(Stack *stack) {
	free(stack->contents);
	stack->contents = NULL;
	stack->maxSize = 0;
	stack->top = -1;
}

/**
 *	Pushes an element to the stack.
 */
void pushStack(Stack *stack, StackElement element) {
	stack->contents[++stack->top] = element;						// Increment top of stack and add element
}

/**
 *	Pops an element from the stack. If the stack is less than half full,
 *	after the pop, its size is halved
 */
StackElement popStack(Stack *stack) {
	if(stackIsEmpty(stack)) {
		fprintf(stderr, "Stack is empty.\n");
	   	exit(1);
	}

	return stack->contents[stack->top--];			// Remove the top element
}

/** 
 *	Return a true value if the stack is empty
 */
int stackIsEmpty(Stack *stack) {
  	return stack->top < 0;
}

/** 
 *	Return a true value if the stack is full
 */
int stackIsFull(Stack *stack) {
  	return stack->top >= stack->maxSize - 1;
}

void printStack(Stack *stack) {
	printf("Stack:\n");
	int c = stack->top;
	while(c >= 0) {
		StackElement e = stack->contents[c];
		printf("%s-%s\n", e.v, e.u);
		c--;
	}
	printf("\n");
}