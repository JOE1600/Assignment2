#include <stdio.h>
#include <stdlib.h>
#include "Graph.h"

void Dijkstra(Graph G, int source) {
	int* dist = (int*)malloc(G.V * sizeof(int)); // Array to store the distances from the source
	int* prev = (int*)malloc(G.V * sizeof(int)); // Array to store the previous vertices in the shortest path

	// Initialization
	for (int i = 0; i < G.V; i++) {
		dist[i] = INT_MAX; // Initialize all distances as infinity
		prev[i] = -1; // Initialize all previous vertices as undefined
	}
	dist[source] = 0; // Distance from the source to itself is 0

	int* Q = (int*)malloc(G.V * sizeof(int)); // Array to store the vertices in the graph
	int Q_size = G.V;

	// Main loop
	while (Q_size > 0) {
		// Find the vertex with the minimum distance from the source
		int min_dist = INT_MAX;
		int u = -1;
		for (int i = 0; i < G.V; i++) {
			if (dist[i] < min_dist && Q[i]) {
				min_dist = dist[i];
				u = i;
			}
		}

		if (u == -1) {
			// No more reachable vertices
			break;
		}

		Q[u] = 0; // Remove the vertex u from Q

		// Update the distances and previous vertices of the neighbors of u
		EdgeNode* curr = G.edges[u].head;
		while (curr != NULL) {
			int v = curr->edge.to_vertex;
			int weight = curr->edge.weight;
			int alt = dist[u] + weight;

			if (alt < dist[v]) {
				dist[v] = alt;
				prev[v] = u;
			}

			curr = curr->next;
		}

		Q_size--;
	}

	// Print the shortest distances from the source
	for (int i = 0; i < G.V; i++) {
		printf("Distance from %d to %d: %d\n", source, i, dist[i]);
	}

	// Clean up memory
	free(dist);
	free(prev);
	free(Q);
}



int main() {
	Graph G;
	scanf_s("%d", &G.V);

	G.edges = (EdgeList*)malloc(G.V * sizeof(EdgeList));

	for (int i = 0; i < G.V; i++) {
		int num_edges;
		scanf_s("%d", &num_edges);

		G.edges[i].head = NULL;

		for (int j = 0; j < num_edges; j++) {
			EdgeNode* new_node = (EdgeNode*)malloc(sizeof(EdgeNode));
			scanf_s("%d,%d", &new_node->edge.to_vertex, &new_node->edge.weight);
			new_node->next = G.edges[i].head;
			G.edges[i].head = new_node;
		}
	}

	int source;
	scanf_s("%d", &source);

	Dijkstra(G, source);

	// Clean up memory
	for (int i = 0; i < G.V; i++) {
		EdgeNode* curr = G.edges[i].head;
		while
			(curr != NULL) {
			EdgeNode* next = curr->next;
			free(curr);
			curr = next;
		}
	}
	free(G.edges);

	return 0;
}


// This will create a graph of the places on the map where user wants to travel with a distance in a day

// post officer will travel 
// it will calculate the nearest distance with the help of dijikra thing


// i will show how depth first search is not good example to be used for a post man where as he can use Prims algo instead