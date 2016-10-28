/*
 * 
 */


#ifndef FIBONACCI_HEAP_H
#define FIBONACCI_HEAP_H

typedef int TBase;

typedef struct fhnode
{
	TBase key;
	struct ftnode* p;
	struct ftnode* child;
	struct ftnode* left;
	struct ftnode* right;
	unsigned degree;
	unsigned char mark : 1;
} FHNode;


typedef struct
{
	FTNode* min; // Указатель на корень дерева с минимальным ключем.
	unsigned n; // Текущее количство узлов.
} FibHeep;


#endif // FIBONACCI_HEAP_H