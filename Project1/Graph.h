#pragma once
#include <stdio.h>
#include <stdlib.h>

typedef struct edge {
	int to_vertex;
	int weight;
} Edge;

typedef struct edgeNode {
	Edge edge;
	struct edgeNode* next;
} EdgeNode;

typedef struct edgeList {
	EdgeNode* head; // changed EdgeNodePtr to EdgeNode
} EdgeList;

typedef struct graph {
	int V;
	EdgeList* edges; // changed to EdgeList* instead of EdgeList
} Graph;