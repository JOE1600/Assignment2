#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "Graph.h"

Subset* createSubset(int v) {
	Subset* subset = (Subset*)malloc((v + 1) * sizeof(Subset));
	for (int i = 0; i <= v; i++) {
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
			edges[i].to_vertex = v;
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
		int y = find(subset, next_edge.weight);

		if (x != y) {
			result[e++] = next_edge;
			unionSets(subset, x, y);
		}
	}

	printf("Minimum Spanning Tree using Kruskal's algorithm:\n");
	for (int j = 0; j < e; j++) {
		printf("%d - %d: weight %d\n", result[j].to_vertex, result[j].weight, result[j].weight);
	}

	free(subset);
	free(edges);
	free(result);
}

void Prim(Graph G) {
	int V = G.V;
	int* parent = (int*)malloc((V + 1) * sizeof(int));
	int* key = (int*)malloc((V + 1) * sizeof(int));
	bool* inMST = (bool*)malloc((V + 1) * sizeof(bool));

	for (int v = 0; v <= V; v++) {
		key[v] = INT_MAX;
		inMST[v] = false;
	}

	key[0] = 0;
	parent[0] = -1;

	for (int count = 0; count < V; count++) {
		int u = -1;
		for (int v = 0; v <= V; v++) {
			if (!inMST[v] && (u == -1 || key[v] < key[u]))
				u = v;
		}

		inMST[u] = true;

		EdgeNode* curr = G.edges[u].head;
		while (curr != NULL) {
			int v = curr->edge.to_vertex;
			int weight = curr->edge.weight;
			if (!inMST[v] && weight < key[v]) {
				parent[v] = u;
				key[v] = weight;
			}
			curr = curr->next;
		}
	}

	printf("Minimum Spanning Tree using Prim's algorithm:\n");
	for (int v = 1; v <= V; v++) {
		printf("Edge: %d - %d\n", parent[v], v);
	}

	free(parent);
	free(key);
	free(inMST);
}

void addEdge(Graph* G, int from_vertex, int to_vertex, int weight) {
	EdgeNode* newNode = (EdgeNode*)malloc(sizeof(EdgeNode));
	newNode->edge.to_vertex = to_vertex;
	newNode->edge.weight = weight;
	newNode->next = G->edges[from_vertex].head;
	G->edges[from_vertex].head = newNode;
}

Graph createGraph(int V) {
	Graph G;
	G.V = V;
	G.edges = (EdgeList*)malloc((V + 1) * sizeof(EdgeList));

	for (int i = 0; i <= V; i++) {
		G.edges[i].head = NULL;
	}

	return G;
}

void printDistance(int* dist, int V) {
	printf("Distance from 0 to:\n");
	for (int i = 1; i <= V; i++) {
		printf("%d: %d\n", i, dist[i]);
	}
}

void printShortestPath(int* parent, int j) {
	if (parent[j] == -1) {
		printf("%d ", j);
		return;
	}
	printShortestPath(parent, parent[j]);
	printf("%d ", j);
}

void printShortestPaths(int* parent, int* dist, int V) {
	printf("Shortest path from 0 to:\n");
	for (int i = 1; i <= V; i++) {
		printf("%d: ", i);
		printShortestPath(parent, i);
		printf("\n");
	}
}

void Dijkstra(Graph G) {
	int V = G.V;
	int* dist = (int*)malloc((V + 1) * sizeof(int));
	bool* visited = (bool*)malloc((V + 1) * sizeof(bool));
	int* parent = (int*)malloc((V + 1) * sizeof(int));

	for (int i = 0; i <= V; i++) {
		dist[i] = INT_MAX;
		visited[i] = false;
		parent[i] = -1;
	}

	dist[0] = 0;

	for (int count = 0; count < V; count++) {
		int u = -1;
		for (int v = 0; v <= V; v++) {
			if (!visited[v] && (u == -1 || dist[v] < dist[u]))
				u = v;
		}

		visited[u] = true;

		EdgeNode* curr = G.edges[u].head;
		while (curr != NULL) {
			int v = curr->edge.to_vertex;
			int weight = curr->edge.weight;
			if (!visited[v] && dist[u] != INT_MAX && dist[u] + weight < dist[v]) {
				dist[v] = dist[u] + weight;
				parent[v] = u;
			}
			curr = curr->next;
		}
	}

	printDistance(dist, V);
	printShortestPaths(parent, dist, V);

	free(dist);
	free(visited);
	free(parent);
}

int main() {
	int V;
	printf("Number of houses: ");
	scanf_s("%d", &V);

	Graph G = createGraph(V);

	for (int i = 0; i < V; i++) {
		int num_neighbors;
		printf("\nEnter the number of neighbors for house %d: ", i);
		scanf_s("%d", &num_neighbors);

		printf("\nEnter the neighbors and weights for house %d:\n", i);
		for (int j = 0; j < num_neighbors; j++) {
			int neighbor, weight;
			scanf_s("%d %d", &neighbor, &weight);
			addEdge(&G, i, neighbor, weight);
		}
	}

	int source;
	printf("\nEnter the source house: ");
	scanf_s("%d", &source);

	Prim(G);
	kruskalMST(G);
	Dijkstra(G);

	return 0;
}



/* breif description of output and input 




Graph Input:

Number of houses: 6

Enter the number of neighbors for house 0: 3

Enter the neighbors and weights for house 0:
1 2
2 3
3 1

Enter the number of neighbors for house 1: 3

Enter the neighbors and weights for house 1:
0 2
2 4
4 5

Enter the number of neighbors for house 2: 3

Enter the neighbors and weights for house 2:
0 3
1 4
3 2

Enter the number of neighbors for house 3: 3

Enter the neighbors and weights for house 3:
0 1
2 2
4 3

Enter the number of neighbors for house 4: 3

Enter the neighbors and weights for house 4:
1 5
3 3
5 2

Enter the number of neighbors for house 5: 2

Enter the neighbors and weights for house 5:
1 1
4 2

Enter the source house: 0

Expected Output(this is what i want):

Minimum Spanning Tree using Prim's algorithm:
Edge: 0 - 1
Edge: 0 - 2
Edge: 2 - 3
Edge: 1 - 4

Minimum Spanning Tree using Kruskal's algorithm:
1 - 0: weight 2
5 - 1: weight 1
3 - 2: weight 2
4 - 1: weight 5

Distance from 0 to 1: 2
Distance from 0 to 2: 3
Distance from 0 to 3: 1
Distance from 0 to 4: 7
Distance from 0 to 5: 6
Shortest path from 0 to 1: 0 1
Shortest path from 0 to 2: 0 2
Shortest path from 0 to 3: 0 3
Shortest path from 0 to 4: 0 1 4
Shortest path from 0 to 5: 0 2 1 5

Distance from 0 to 1: 2
Distance from 0 to 2: 3
Distance from 0 to 3: 1
Distance from 0 to 4: 7
Distance from 0 to 5: 6
Shortest path from 0 to 1: 0 1
Shortest path from 0 to 2: 0 2
Shortest path from 0 to 3: 0 3
Shortest path from 0 to 4: 0 1 4
Shortest path from 0 to 5: 0 2 1 5

Shortest path from 0 to 1: 0 1
Shortest path from 0 to 2: 0 2
Shortest path from 0 to 3: 0 3
Shortest path from 0 to 4: 0 1 4
Shortest path from 0 to 5: 0 2 1 5





















*/