#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "graph.h"

/**
 *	Creates a graph of v vertices
 */
Graph * initGraph(int v) {
	Graph *graph = (Graph *) malloc(sizeof(Graph));				// Allocate memory for the graph
	if(!graph) {
		fprintf(stderr, "Insufficient memory to init graph.\n");
	 	exit(1);
    }

    graph->numVertices = v;
    graph->emptyIndex = 0;
    graph->numEdges = 0;

    // Create the array of adjacency lists
    graph->array = (Vertex *) malloc(v * sizeof(Vertex));

    // Initialise each adjacency list as empty
    int i;
    for(i = 0; i < v; ++i) {
    	graph->array[i].adjListHead = NULL;
    }

    return graph;
}

/**
 *	Frees the nodes, array, and graph itself
 */
void destroyGraph(Graph *graph) {
	if(graph) {
		int i;
		for(i = 0; i < graph->emptyIndex; i++) {
			Vertex *v = &graph->array[i];
			AdjListNode *adj = v->adjListHead;
			while(adj) {
				AdjListNode *tmp = adj;
				adj = adj->next;
				free(tmp);
			}
		}
        free(graph->array);
		free(graph);
	}
}

/**
 *  Utility function to create an ajdacency list node
 */
AdjListNode * newAdjListNode(int index) {
    AdjListNode *newNode = (AdjListNode *) malloc(sizeof(AdjListNode));
    newNode->index = index;
    newNode->next = NULL;
    return newNode;
}

/**
 *  Creates a vertex with the given label and adds it to the graph
 *  It is assumed that the label given does not already exists
 */
int addVertex(Graph *graph, char *label) {
    char *labelCopy = malloc(strlen(label) + 1);
    strcpy(labelCopy, label);

    // Vertex doesn't already exist - create it
    if(graph->emptyIndex >= graph->numVertices) {
        fprintf(stderr, "Graph is full\n");
        exit(1);
    }

    // Insert at the empty index and increment it for next insertion
    int index = graph->emptyIndex;
    graph->array[index].label = labelCopy;

    graph->emptyIndex++;
    return index;
}

/**
 *  Returns the index of the vertex with the given label. If
 *  not found, -1 is returned.
 */
int getVertex(Graph *graph, char* label) {
    int i;
    Vertex *v;
    for(i = 0; i < graph->emptyIndex; i++) {
        v = &graph->array[i];
        if(!strcmp(label, v->label)) {
            // Vertex exists
            return i;
        }
    }
    return -1;
}

/**
 *	Adds the given vertex to the graph, but first searches if it already exists.
 *  If so, the index of the vertex is returned.
 */
int addVertexSearch(Graph *graph, char* label) {
	// First check if there's a vertex that already contains the given label
	int v = getVertex(graph, label);
    if(v != -1) {
        // Not found - add it
        return addVertex(graph, label);
    } else {
        // Vertex was found
        return v;
    }
}

/**
 *  Adds an edge between the vertices at src and dest
 *  As graph input is expected to define both edge directions
 *  (i.e. src-dest and dest-src), only one direction is added here
 */
void addEdge(Graph *graph, int src, int dest) {
	AdjListNode *newNode = newAdjListNode(dest);
    newNode->next = graph->array[src].adjListHead;
    graph->array[src].adjListHead = newNode;
    graph->numEdges++;
}

/**
 *  Prints out each vertex and its adjacency list
 */ 
void displayGraph(Graph *graph) {
    int i;
    for(i = 0; i < graph->emptyIndex; i++) {
        printf("%s", graph->array[i].label);
        AdjListNode *node = graph->array[i].adjListHead;
        while(node) {
            printf("-%s", graph->array[node->index].label);
            node = node->next;
        }
        printf("\n");
    }
}

/**
 *  Prints out each vertex and its adjacency list to the given file
 */
void outputGraph(Graph *graph, FILE *fp) {
    int i;
    for(i = 0; i < graph->emptyIndex; i++) {
        fprintf(fp, "%s", graph->array[i].label);
        AdjListNode *node = graph->array[i].adjListHead;
        while(node) {
            fprintf(fp, "-%s", graph->array[node->index].label);
            node = node->next;
        }
        fprintf(fp, "\n");
    }
}

/**
 *  Returns the out degree of the given vertex in the graph
 */
int outDegree(Graph *graph, int index) {
    Vertex *v = &graph->array[index];
    AdjListNode *u = v->adjListHead;
    int degree = 0;
    while(u) {
        degree++;
        u = u->next;
    }
    return degree;
}

/**
 *  Returns true if dest is reachable from src
 */
int isReachable(Graph *graph, int src, int dest) {
    Vertex *srcVertex = &graph->array[src];
    AdjListNode *u = srcVertex->adjListHead;

    while(u) {
        if(u->index == dest) {
            return 1;
        } 
        u = u->next;
    }

    return 0;
}

/**
 *  Returns the number of distinct edges in the graph
 *  I.e., the edges a-b and b-a are counted as one
 *  Thus, the edge count is halved
 */
int numEdges(Graph *graph) {
    return graph->numEdges / 2;
}