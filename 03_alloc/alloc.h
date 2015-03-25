typedef unsigned int NSUInteger;
typedef int NSInteger;

void *customAlloc(NSUInteger);
void *customRealloc(void *, NSUInteger);
void customFree(void *);

void printBlock(void *);