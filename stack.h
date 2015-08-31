typedef struct {
	char * v;
	char * u;
} StackElement;

typedef struct stack {
	StackElement *contents;		// Array to hold contents of stack
  	int maxSize;				
  	int top;					// Index of the top element of the stack in the array
} Stack;

/**
 *	Initialises a new, empty, stack of maxSize size
 */
Stack * initStack(int maxSize);

/**
 *	Frees all memory associated with the stack. Stack cannot
 *	be used again until reinitialised
 */
void destroyStack(Stack *stack);

/**
 *	Pushes an element to the stack. If the stack is full,
 *	its size is doubled before pushing
 */
void pushStack(Stack *stack, StackElement element);

/**
 *	Pops an element from the stack. If the stack is less than half full,
 *	after the pop, its size is halved
 */
StackElement popStack(Stack *stack);

/** 
 *	Return a true value if the stack is empty or full (respectively).
 */
int stackIsEmpty(Stack *stack);
int stackIsFull(Stack *stack);
void printStack(Stack *stack);