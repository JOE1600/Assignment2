#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>


typedef struct edge {
	int to_vertex;
	int weight;
} Edge;

typedef struct edgeNode {
	Edge edge;
	struct edgeNode* next;
} EdgeNode;

typedef struct edgeList {
	EdgeNode* head;
} EdgeList;

typedef struct graph {
	int V;
	EdgeList* edges;
} Graph;

typedef struct subset {
	int parent;
	int rank;
} Subset;