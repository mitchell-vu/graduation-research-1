#ifndef _GRAPH_JRB_H_
#define _GRAPH_JRB_H_

#include "../dllist/dllist.h"
#include "../jrb/jrb.h"

#define BOOL_TRUE      1
#define BOOL_FALSE     0
#define BOOL_NOT_FOUND -1

#define UNDIRECTED 0
#define DIRECTED   1

#define INFINITIVE_VALUE 100000000

typedef struct {
  JRB edges;
  JRB vertices;
  int type;
} Graph;

// ------------------------------------------------------------
// CREATE - DESTROY functions
// ------------------------------------------------------------

/**
 * @brief Create a Graph object
 *
 * @param type UNDIRECTED or DIRECTED
 * @return Graph
 */
extern Graph createGraph(int type);

/**
 * @brief Free a Graph object
 *
 * @param graph
 */
extern void dropGraph(Graph graph);

// ------------------------------------------------------------
// ADD - DELETE functions
// ------------------------------------------------------------

extern void addVertex(Graph graph, Jval id, char *name);
extern void addEdge(Graph graph, Jval v1, Jval v2, double weight);
extern void deleteVertex(Graph graph, Jval v);
extern void deleteEdge(Graph graph, Jval v1, Jval v2);
extern void updateWeight(Graph graph, Jval v1, Jval v2, double newWeight);

// ------------------------------------------------------------
// QUERY functions
// ------------------------------------------------------------

/**
 * @brief Get the vertex value
 *
 * @param graph
 * @param id Vertex ID
 * @return Vertex name or NULL if not found
 */
extern char *getVertex(Graph graph, Jval id);

/**
 * @brief Get the vertex ID
 *
 * @param graph
 * @param name
 * @return Vertex ID or -1 if not found
 */
extern int getVertexId(Graph graph, char *name);

/**
 * @brief Get the edge weight
 *
 * @param graph
 * @param v1 1st vertex ID
 * @param v2 2nd vertex ID
 * @return Edge weight or INFINITIVE_VALUE if not found or the graph is undirected
 */
extern double getEdgeValue(Graph graph, Jval v1, Jval v2);

/**
 * @brief Get the number of vertices
 *
 * @param graph
 * @return int
 */
extern int getVertexNum(Graph graph);

/**
 * @brief Get the number of edges
 *
 * @param graph
 * @return int
 */
extern int getEdgeNum(Graph graph);

/**
 * @brief Get the indegree of a vertex.
 * Which is the number of head ends adjacent to a vertex
 *
 * @param graph
 * @param v Vertex ID Jval
 * @param output Array of indegree vertices
 * @return Return the number of indegree vertices
 *        or -1 if the vertex is not found
 */
extern int indegree(Graph graph, Jval v, long *output);

/**
 * @brief Get the indegree of a vertex.
 *        Which is the number of tail ends adjacent to a vertex
 *
 * @param graph
 * @param v Vertex ID Jval
 * @param output Array of outdegree vertices
 * @return Return the number of outdegree vertices
 *         or -1 if the vertex is not found
 */
extern int outdegree(Graph graph, Jval v, long *output);

// ------------------------------------------------------------
// CHECK functions: hasEdge,... Return boolean
// ------------------------------------------------------------

/**
 * @brief Check if is there any edge between 2 vertices
 *
 * @param graph
 * @param v1 1st vertex Jval object
 * @param v2 2nd vertex Jval object
 * @return yes - 1 or no - 0
 */
extern int hasEdge(Graph graph, Jval v1, Jval v2);

// ------------------------------------------------------------
// GRAPH TRAVESAL functions
// ------------------------------------------------------------

extern int DFS(Graph graph, long graph_size, long start, long stop, long *path);
extern int connectedComponents(Graph graph, long graph_size, FILE *output_fptr);

#endif