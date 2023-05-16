#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"

int main() {
	Graph G;
	scanf_s("%d", &G.V);
	G.edges = (EdgeList*)malloc(G.V * sizeof(EdgeList*));
	// allocate memory for an array of EdgeLists
	for (int i = 0; i < G.V; i++) {
		int num_edges;
		scanf_s("%d", &num_edges);
		G.edges[i].head = NULL;
		// initialize the head of the ith edge list to NULL
		for (int j = 0; j < num_edges; j++) {
			EdgeNode* new_node = (EdgeNode*)malloc(sizeof(EdgeNode));
			// create a new node
			scanf_s("%d,%d", &new_node->edge.to_vertex, &new_node->edge.weight);
			// scan the edge to-vertex and weight
			new_node->next = G.edges[i].head;
			// insert the new node at the front of the current list
			G.edges[i].head = new_node;
		}
	}
	return 0;
}
