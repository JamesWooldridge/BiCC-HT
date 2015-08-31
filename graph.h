typedef struct adjListNode {
	int index;							// The index of this node in the graphs' array of nodes
	struct adjListNode *next;			// The next AdjListNode in the adjacency list
} AdjListNode;

typedef struct vertex {
	int index;							// The index of this node in the graphs' array of nodes
	char* label;						// The label of the node of this adjacency list
	AdjListNode *adjListHead;			// Pointer to head node of list

	/* Biconnected component search relevant properties */
	int depth, lowpoint, visited;
	struct vertex *parent;
} Vertex;

typedef struct graph {
	int numVertices;					// The number of vertices in the graph
	Vertex* array;						// Array of vertices and their adjacency lists
	int emptyIndex;						// Index to the next empty spot in the array
	int numEdges;
} Graph;

/**
 *	Graph creation / destruction
 */
Graph * initGraph(int n);
void destroyGraph(Graph *graph);

/**
 *	Graph manipulation
 */
int addVertex(Graph *graph, char* label);
int addVertexSearch(Graph *graph, char* label);
int getVertex(Graph *graph, char* label);
void addEdge(Graph *graph, int src, int dest);

void displayGraph(Graph *graph);
void outputGraph(Graph *graph, FILE *fp);
int outDegree(Graph *graph, int v);
int isReachable(Graph *graph, int src, int dest);
int numEdges(Graph *graph);