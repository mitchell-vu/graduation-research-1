#ifndef _GRAPH_JRB_H_
#define _GRAPH_JRB_H_

#include "../jrb/jrb.h"
#include "../dllist/dllist.h"

#define BOOL_TRUE 1
#define BOOL_FALSE 0

#define UNDIRECTED 0
#define DIRECTED 1

#define INFINITIVE_VALUE 100000000

typedef struct
{
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

extern void addVertex(Graph graph, int id, char *name);
extern void addEdge(Graph graph, int v1, int v2, double weight);
extern void deleteVertex(Graph graph, int v);
extern void deleteEdge(Graph graph, int v1, int v2);
extern void updateWeight(Graph graph,
                         int v1,
                         int v2,
                         double newWeight);

// ------------------------------------------------------------
// QUERY functions
// ------------------------------------------------------------

extern void *getVertex(Graph graph, // Get vertex value
                       int id);
extern int getVertexId(Graph graph,     // Get vertex id by value
                       char *name);     // Return -1 if not found
extern double getEdgeValue(Graph graph, // Get egde weight
                           int v1,
                           int v2);

extern int getVertexNum(Graph graph); // Đếm số đỉnh
extern int getEdgeNum(Graph graph);   // Đếm số cạnh

extern int indegree(Graph graph, // Cho ra array output
                    int v,       // các nodes đi vào đỉnh v
                    int *output);

extern int outdegree(Graph graph, // Cho ra array output
                     int v,       // các nodes đi ra từ đỉnh v
                     int *output);

// ------------------------------------------------------------
// CHECK functions: isDAG? hasEdge? ...
// ------------------------------------------------------------

extern int hasEdge(Graph graph, // yes - 1 ; no - 0
                   int v1,
                   int v2);

extern int DAG(Graph graph); // yes - 1 ; no - 0
                             // Have cycle -> return 0

extern int DFS_DAG(Graph graph, int start);

// ------------------------------------------------------------
// GRAPH TRAVESAL functions:
// ------------------------------------------------------------

extern double shortestPath(Graph graph, // Dijisktra using BFS
                           int start,
                           int stop,
                           int *path,
                           int *numVertices);

extern int DFS(Graph graph,
               int start,
               int stop,
               int *path);

extern int BFS(Graph graph, // Shortest no weight path
               int start,
               int stop,
               int *path);

extern int topologicalSort(Graph graph,  // return total
                           int *output); // number of nodes

#endif