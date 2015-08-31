#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include "graph.h"
#include "stack.h"

#define MIN(X,Y) ((X) < (Y) ? (X) : (Y))

/**
 *	The number of vertices in the graph
 */
int graphVertices = 0;

/**
 *	Parses the graph defined in the given file and generates a graph
 *	If intMode is true, then it is assumed the vertex labels can be processed
 *	as integers, allowing for quicker processing
 */
Graph * parseInput(char* filename, int intMode) {
	FILE *file = fopen(filename, "r");
	Graph *graph = NULL;

	if(file != NULL) {
		char line[1000];
		int lineNum = 0;
		while(fgets(line, sizeof line, file) != NULL) {
			// Strip the trailing new line character
			size_t ln = strlen(line) - 1;
			if(line[ln] == '\n') {
				line[ln] = '\0';
			}

			if(lineNum == 0) {
				// First line - get the number of vertices
				int numVertices = strtol(line, NULL, 10);
				graphVertices = numVertices;
				graph = initGraph(numVertices);
			} else if(lineNum <= graphVertices) {
				// Parsing the vertex labels
				addVertex(graph, line);
			} else {
				// Get the source vertex
				char *v = strtok(line, "-");
				int src;

				/**
				 *	If int mode is true, we can use the label of the vertex as the index into
				 *	the graphs' array of vertices, rather than doing a string based search
				 */
				if(intMode) {
					// convert v to int
					src = strtol(v, NULL, 10);
				} else {
					src = getVertex(graph, v);
				}
				
				// Go through each vertex adjacent to the source and add the edges
				v = strtok(NULL, "-");
				while(v != NULL) {
					int dest;
					if(intMode) {
						// convert v to int
						dest = strtol(v, NULL, 10);
					} else {
						dest = getVertex(graph, v);
					}
					addEdge(graph, src, dest);
					v = strtok(NULL, "-");
				}
			}

			lineNum++;
		}

		fclose(file);
		return graph;
	} else {
		fprintf(stderr, "Could not open input file\n");
		return NULL;
	}
}

/**
 *	Repeatedly pops edges off of the given stack until the given edge is found
 *	The edges are used to create a graph (i.e. the biconnected component),
 *	which is then written to the output file
 */
void outputComp(StackElement edge, Stack *stack, FILE *fp) {
	Graph *g = initGraph(graphVertices);
	StackElement e;
	do {
		e = popStack(stack);

		int v = addVertexSearch(g, e.v);
		int u = addVertexSearch(g, e.u);

		// Add each edge that isn't the one we're looking for to the graph
		addEdge(g, v, u);
		addEdge(g, u, v);
	} while(!(strcmp(e.v, edge.v) == 0 && strcmp(e.u, edge.u) == 0));

	// Biconnected comonent subgraph finished - now output it to the file
	outputGraph(g, fp);
	fprintf(fp, "\n");

	destroyGraph(g);
}

/**
 *	Performs a DFS on the given graph, searching for articulation points. As each
 *	vertex is encountered, its outgoing edges are pushed on to a stack (before
 *	continuing the search from its descendents). After the recursive search from 
 *	a child u of vertex v is finished, we check if v is an articulation point. If 
 *	it is, all edges on the stack up to (and including) (v, u) are popped and output
 *	to a file as a biconnected component.
 */
void dfsArticulation(Graph *graph, Vertex *v, int depth, Stack *stack, FILE *fp) {
	v->visited = 1;
	v->depth = depth;
	v->lowpoint = depth;

	// Search each child u from vertex v
	AdjListNode *uAdj = v->adjListHead;
	while(uAdj) {
		Vertex *u = &graph->array[uAdj->index];

		// Form an edge (v, u) to put on the stack
		StackElement e = {v->label, u->label};

		if(!u->visited) {
			pushStack(stack, e);
			u->parent = v;

			// Recursively search from u with an increased depth
			dfsArticulation(graph, u, depth + 1, stack, fp);
			
			if(u->lowpoint >= v->depth) {
				// Articulation point found - output all edges up to e
				outputComp(e, stack, fp);
			}

			v->lowpoint = MIN(v->lowpoint, u->lowpoint);
		} else if((u->depth < v->depth) && (u != v->parent)) {
			// u has been visited, but we still need to update v's lowpoint with information from u
			pushStack(stack, e);
			v->lowpoint = MIN(v->lowpoint, u->depth);
		}

		uAdj = uAdj->next;
	}
}

/**
 *	Outputs the biconnected components of the given graph
 *
 *	First, the graph is prepared for the dfsArticulation function by
 *	resetting each nodes' visited, parent, depth, and lowpoint members
 */
void findBiconnectedComponents(Graph *graph, FILE *fp) {
	int i;
	Vertex *v;
	for(i = 0; i < graph->emptyIndex; i++) {
		v = &graph->array[i];
		v->visited = 0;
		v->parent = 0;
		v->depth = 0;
		v->lowpoint = 0;
	}

	v = &graph->array[0];

	Stack *stack = initStack(graph->numEdges);
	dfsArticulation(graph, v, 0, stack, fp);
	destroyStack(stack);
}

/**
 *	Finds the biconnected components in a graph described by the given
 *	input file, and outputs them to the specified output file
 *	If intMode is true, then it is assumed the vertex labels in the input
 *	graph can be processed as integers, allowing for quicker processing
 */
int main(int argc, char *argv[]) {
	// Check that all params are given
	if(argc != 4) {								
		fprintf(stderr, "Usage: ./bic graphFile outputFile intMode\n");
		return 1;
	}

	char* filename = argv[1];
	FILE *output = fopen(argv[2], "w");
	int intMode = strtol(argv[3], NULL, 10);
	Graph *graph = parseInput(filename, intMode);
	if(graph) {
		clock_t begin, end;	
		double time_spent;
		
		begin = clock();
		findBiconnectedComponents(graph, output);
		end = clock();

		time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
		printf("%0.5f\n", time_spent);

		destroyGraph(graph);
	} else {
		fprintf(stderr, "Error: graph could not be created\n");
		return 1;
	}

	fclose(output);
}