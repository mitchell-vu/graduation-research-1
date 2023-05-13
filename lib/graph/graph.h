#ifndef _GRAPH_JRB_H_
#define _GRAPH_JRB_H_

#include "../dllist/dllist.h"
#include "../jrb/jrb.h"

#define BOOL_TRUE  1
#define BOOL_FALSE 0

#define UNDIRECTED 0
#define DIRECTED   1

#define INFINITIVE_VALUE 100000000

typedef struct {
  JRB edges;
  JRB vertices;
  int type;
} Graph;

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

extern void addVertex(Graph graph, long id, char *name);
extern void addEdge(Graph graph, long v1, long v2, double weight);
extern void deleteVertex(Graph graph, int v);
extern void deleteEdge(Graph graph, int v1, int v2);
extern void updateWeight(Graph graph, int v1, int v2, double newWeight);

// ------------------------------------------------------------
// QUERY functions
// ------------------------------------------------------------

/**
 * @brief Get the vertex value
 *
 * @param graph
 * @param id
 * @return Jval
 */
extern char *getVertex(Graph graph, long id);

/**
 * @brief Get the vertex ID
 *
 * @param graph
 * @param name
 * @return Vertex ID or -1 if not found
 */
extern int getVertexId(Graph graph, char *name);
extern double getEdgeValue(Graph graph, int v1, int v2);

extern int getVertexNum(Graph graph);   // Đếm số đỉnh
extern int getEdgeNum(Graph graph);     // Đếm số cạnh

extern int indegree(Graph graph,   // Cho ra array output
                    int v,         // các nodes đi vào đỉnh v
                    int *output);

extern int outdegree(Graph graph,   // Cho ra array output
                     long v,        // các nodes đi ra từ đỉnh v
                     long *output);

// ------------------------------------------------------------
// CHECK functions: isDAG? hasEdge? ...
// ------------------------------------------------------------

/**
 * @brief Check if the graph has an edge of given vertices
 *
 * @param graph
 * @param v1
 * @param v2
 * @return yes - 1 or no - 0
 */
extern int hasEdge(Graph graph, Jval v1, Jval v2);

extern int DFS(Graph graph, long graph_size, long start, long stop, long *path);
extern int connectedComponents(Graph graph, long graph_size, char *filename);

#endif