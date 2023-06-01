#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
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

void Prim(Graph G, int source) {
	int* dist = (int*)malloc(G.V * sizeof(int)); // Array to store the distances from the current solution
	int* prev = (int*)malloc(G.V * sizeof(int)); // Array to store the previous vertices in the MST

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
		// Find the vertex with the minimum distance from the current solution
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

			if (weight < dist[v]) {
				dist[v] = weight;
				prev[v] = u;
			}

			curr = curr->next;
		}

		Q_size--;
	}

	// Print the edges in the Minimum Spanning Tree (MST)
	printf("Minimum Spanning Tree using Prim's algorithm:\n");
	for (int i = 0; i < G.V; i++) {
		if (prev[i] != -1) {
			printf("Edge: %d - %d\n", prev[i], i);
		}
	}

	// Clean up memory
	free(dist);
	free(prev);
	free(Q);
}

Subset* createSubset(int v) {
	Subset* subset = (Subset*)malloc(v * sizeof(Subset));
	for (int i = 0; i < v; i++) {
		subset[i].parent = i;
		subset[i].rank = 0;
	}
	return subset;
}

int find(Subset* subset, int i) {
	if (subset[i].parent != i)
		subset[i].parent = find(subset, subset[i].parent);
	return subset[i].parent;
}

void unionSets(Subset* subset, int x, int y) {
	int xroot = find(subset, x);
	int yroot = find(subset, y);
	if (subset[xroot].rank < subset[yroot].rank)
		subset[xroot].parent = yroot;
	else if (subset[xroot].rank > subset[yroot].rank)
		subset[yroot].parent = xroot;
	else {
		subset[yroot].parent = xroot;
		subset[xroot].rank++;
	}
}

int compareEdges(const void* a, const void* b) {
    Edge* edgeA = (Edge*)a;
    Edge* edgeB = (Edge*)b;
    return edgeA->weight - edgeB->weight;
}

void kruskalMST(Graph G) {
	int V = G.V;
	Edge* result = (Edge*)malloc((V - 1) * sizeof(Edge));
	int e = 0;
	int i = 0;

	Subset* subset = createSubset(V);
	Edge* edges = (Edge*)malloc(V * V * sizeof(Edge));

	for (int v = 0; v < V; v++) {
		EdgeNode* curr = G.edges[v].head;
		while (curr != NULL) {
			edges[i].to_vertex = curr->edge.to_vertex;
			edges[i].weight = curr->edge.weight;
			curr = curr->next;
			i++;
		}
	}

	qsort(edges, i, sizeof(Edge), compareEdges);

	i = 0;
	while (e < V - 1 && i < V * V) {
		Edge next_edge = edges[i++];
		int x = find(subset, next_edge.to_vertex);
		int y = find(subset, next_edge.to_vertex);


		if (x != y) {
			result[e++] = next_edge;
			unionSets(subset, x, y);
		}
	}

	printf("Minimum Spanning Tree using Kruskal's algorithm:\n");
	for (i = 0; i < e; i++) {
		printf("%d - %d: weight %d\n", result[i].to_vertex, result[i].weight);
	}

	free(result);
	free(subset);
	free(edges);
}

int main() {
	Graph G;
	printf("Enter the number of houses: ");
	scanf_s("%d", &G.V);

	G.edges = (EdgeList*)malloc(G.V * sizeof(EdgeList));

	for (int i = 0; i < G.V; i++) {
		int num_edges;
		printf("Enter the number of neighbors for house %d: ", i);
		scanf_s("%d", &num_edges);

		G.edges[i].head = NULL;

		printf("Enter the neighbors and weights for house %d:\n", i);
		for (int j = 0; j < num_edges; j++) {
			EdgeNode* new_node = (EdgeNode*)malloc(sizeof(EdgeNode));
			scanf_s("%d %d", &new_node->edge.to_vertex, &new_node->edge.weight);
			new_node->next = G.edges[i].head;
			G.edges[i].head = new_node;
		}
	}

	int source;
	printf("Enter the source house: ");
	scanf_s("%d", &source);

	// Run Prim's algorithm
	Prim(G, source);

	// Run Kruskal's algorithm
	kruskalMST(G);

	// Calculate the shortest distances between houses using Dijkstra's algorithm
	for (int i = 0; i < G.V; i++) {
		if (i != source) {
			Dijkstra(G, source, i);
		}
	}

	// Clean up memory
	for (int i = 0; i < G.V; i++) {
		EdgeNode* curr = G.edges[i].head;
		while (curr != NULL) {
			EdgeNode* next = curr->next;
			free(curr);
			curr = next;
		}
	}

	free(G.edges);
	return 0;
}







