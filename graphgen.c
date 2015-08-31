#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "graph.h"

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))

void generateBiconnectedGraph(int n, int c, char *graphOutput, char *verificationOutput);
Graph * generateBiconnectedComponent(int min, int max, FILE *fp);
int numArticulationPoints(Graph *graph);
int dfsArticulation(Graph *graph, Vertex *v, int depth, int numPoints);
void buildRandomEdge(Graph *graph);


void generateBiconnectedGraph(int n, int c, char *graphOutput, char *verificationOutput) {
	FILE *gOut= fopen(graphOutput, "w");
	FILE *vOut= fopen(verificationOutput, "w");

	int cn = n / c;
	int i;
	int max = 0;
	int min = 0;
	
	fprintf(gOut, "%d\n", n);
	for(i = 0; i < n; i++) {
		fprintf(gOut, "%d\n", i);
	}
	for(i = 0; i < c; i++) {
		// Build a biconnected component
		min = i * cn;
		max = min + cn;

		Graph *component = generateBiconnectedComponent(min, max, vOut);

		// Output the edges of the component
		int v;
		for(v = 0; v < component->numVertices; v++) {
			fprintf(gOut, "%s", component->array[v].label);
			AdjListNode *node = component->array[v].adjListHead;
			while(node) {
		        fprintf(gOut, "-%s", component->array[node->index].label);
		        node = node->next;
		    }
		    // Connect it to the last component
		    if(i > 0 && v == 0) {
				fprintf(gOut, "-%d", (min - 1));

				// Also output this egde as an extra biconnected component
				fprintf(vOut, "%d-%d\n", min, (min-1));
				fprintf(vOut, "%d-%d\n\n", (min-1), min);
			}
		    if(v < component->numVertices - 1) {
		    	fprintf(gOut, "\n");
		    }
		}

		if(i < c - 1) {
			// Add an edge between the first node of the next component and the last edge of this component
			fprintf(gOut, "-%d\n", max);
		}

		destroyGraph(component);
	}

	fclose(gOut);
	fclose(vOut);
}

/**
 *	Generates a biconnected component with n vertices (labelled between min and max)
 */
Graph * generateBiconnectedComponent(int min, int max, FILE *fp) {
	Graph *graph = NULL;
	graph = initGraph(max - min);

	// Create n vertices
	int v = min;
	while(v < max) {
		char str[15];
		sprintf(str, "%d", v);
		int vert = addVertex(graph, str);
		v++;
	}

	int a = 0;
	while(a != 1) {
		buildRandomEdge(graph);

		a = numArticulationPoints(graph);
	}

	// Write the graph to the bic output file
	int i;
	for(i = 0; i < graph->numVertices; i++) {
	    fprintf(fp, "%s", graph->array[i].label);
	    AdjListNode *node = graph->array[i].adjListHead;
	    while(node) {
	        fprintf(fp, "-%s", graph->array[node->index].label);
	        node = node->next;
	    }
	    fprintf(fp, "\n");
	}

	fprintf(fp, "\n");

	return graph;
}

/**
 *	Picks two random nodes that don't share and edge and adds an edge
 *	between them
 */
void buildRandomEdge(Graph *graph) {
	// Pick a random src node that has room for another edge
	int srcIndex = rand() % graph->emptyIndex;
	while(outDegree(graph, srcIndex) >= graph->emptyIndex - 1) {
		srcIndex = rand() % graph->emptyIndex;
	}

	// Pick a dest node that isn't already connected to the src node
	int destIndex  = rand() % graph->emptyIndex;
	int alreadyChecked[graph->emptyIndex];
	int i;
	for(i = 0; i < graph->emptyIndex; i++) { 
		alreadyChecked[i] = 0;
	}
	while(destIndex == srcIndex || alreadyChecked[destIndex] == 1 || isReachable(graph, srcIndex, destIndex)) {
		alreadyChecked[destIndex] = 1;
		destIndex = rand() % graph->emptyIndex;
	}


	// Now have unconnected src and dest nodes - build the edge
	addEdge(graph, srcIndex, destIndex);
	addEdge(graph, destIndex, srcIndex);
}

int dfsArticulation(Graph *graph, Vertex *v, int depth, int numPoints) {
	v->visited = 1;
	v->depth = depth;
	v->lowpoint = depth;

	AdjListNode *uAdj = v->adjListHead;
	while(uAdj) {
		Vertex *u = &graph->array[uAdj->index];

		if(!u->visited) {
			u->parent = v;

			numPoints = dfsArticulation(graph, u, depth + 1, numPoints);

			if(u->lowpoint >= v->depth) {
				// Articulation point found
				numPoints++;
			}

			v->lowpoint = MIN(v->lowpoint, u->lowpoint);
		} else if((u->depth < v->depth) && (u != v->parent)) {
			v->lowpoint = MIN(v->lowpoint, u->depth);
		}

		uAdj = uAdj->next;
	}

	return numPoints;
}

int numArticulationPoints(Graph *graph) {
	int i;
	for(i = 0; i < graph->emptyIndex; i++) {
		Vertex *v = &graph->array[i];
		v->visited = 0;
		v->parent = 0;
		v->depth = 0;
		v->lowpoint = 0;
	}

	Vertex *v = &graph->array[0];
	int artPoints = dfsArticulation(graph, v, 0, 0);

	for(i = 0; i < graph->emptyIndex; i++) {
		Vertex *v = &graph->array[i];
		if(!v->visited) {
			// Not connected
			return 2;
		}
	}

	return artPoints;
}

int main(int argc, char *argv[]) {
	int numVertices = strtol(argv[1], NULL, 10);
	int numComponents = strtol(argv[2], NULL, 10);
	char *graphOutput = argv[3];
	char *verificationOutput = argv[4];
	generateBiconnectedGraph(numVertices, numComponents, graphOutput, verificationOutput);
}