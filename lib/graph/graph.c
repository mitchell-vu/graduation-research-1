#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"

int compareLongInt(Jval j1, Jval j2) {
  long l1 = j1.l;
  long l2 = j2.l;

  if (l1 > l2)
    return 1;
  if (l1 < l2)
    return -1;
  return 0;
}

void reverseArray(long *arr, int size) {
  for (int i = 0; i < size / 2; i++) {
    int c = arr[i];
    arr[i] = arr[size - i - 1];
    arr[size - i - 1] = c;
  }
}

// ------------------------------------------------------------
// CREATE - DESTROY functions
// ------------------------------------------------------------

Graph createGraph(int type) {
  Graph g;
  g.vertices = make_jrb();
  g.edges = make_jrb();
  g.type = type;

  return g;
}

//-----------------------------------------

void dropGraph(Graph graph) {
  JRB node;

  // Free edges tree
  jrb_traverse(node, graph.edges) jrb_free_tree((JRB) jval_v(node->val));
  jrb_free_tree(graph.edges);

  // Free vertices tree
  jrb_free_tree(graph.vertices);
}

// ------------------------------------------------------------
// ADD - DELETE functions
// ------------------------------------------------------------

void addVertex(Graph graph, Jval v, char *name) {
  JRB node = jrb_find_gen(graph.vertices, v, compareLongInt);
  if (node == NULL)
    jrb_insert_gen(graph.vertices, v, new_jval_s(strdup(name)), compareLongInt);
}

//-----------------------------------------

void deleteVertex(Graph graph, Jval v) {
  JRB node_v = jrb_find_gen(graph.vertices, v, compareLongInt);
  jrb_delete_node(node_v);

  // Delete all edges connect to v
  JRB node_e = jrb_find_gen(graph.edges, v, compareLongInt);
  JRB tree = (JRB) jval_v(node_e->val);
  jrb_free_tree(tree);
  jrb_delete_node(node_e);
}

//-----------------------------------------

void updateVertex(Graph graph, Jval v, char *newName) {
  deleteVertex(graph, v);
  addVertex(graph, v, newName);
}

//-----------------------------------------

void addEdge(Graph graph, Jval v1, Jval v2, double weight) {
  if (getVertex(graph, v1) == NULL) {
    char *name = (char *) malloc(20 * sizeof(char));
    sprintf(name, "%ld", jval_l(v1));
    addVertex(graph, v1, name);
    free(name);
  }

  if (getVertex(graph, v2) == NULL) {
    char *name = (char *) malloc(20 * sizeof(char));
    sprintf(name, "%ld", jval_l(v2));
    addVertex(graph, v2, name);
    free(name);
  }

  JRB node, tree;
  int allowToAdd;

  if (graph.type == DIRECTED) {
    if (getEdgeValue(graph, v1, v2) == INFINITIVE_VALUE)
      allowToAdd = BOOL_TRUE;
    else
      allowToAdd = BOOL_FALSE;
  } else {
    if (getEdgeValue(graph, v1, v2) == INFINITIVE_VALUE || getEdgeValue(graph, v2, v1) == INFINITIVE_VALUE)
      allowToAdd = BOOL_TRUE;
    else
      allowToAdd = BOOL_FALSE;
  }

  if (allowToAdd == BOOL_TRUE) {
    node = jrb_find_gen(graph.edges, v1, compareLongInt);

    // Check if v1 node have a tree
    if (node == NULL) {
      tree = make_jrb();
      jrb_insert_gen(graph.edges, v1, new_jval_v(tree), compareLongInt);
    } else
      tree = (JRB) jval_v(node->val);

    jrb_insert_gen(tree, v2, new_jval_d(weight), compareLongInt);

    if (graph.type == UNDIRECTED)
      addEdge(graph, v2, v1, weight);
  }
}

//-----------------------------------------

void deleteEdge(Graph graph, Jval v1, Jval v2) {
  JRB node1;
  JRB node2;

  if (getEdgeValue(graph, v1, v2) != INFINITIVE_VALUE) {
    node1 = jrb_find_gen(graph.edges, v1, compareLongInt);

    JRB tree = (JRB) jval_v(node1->val);
    node2 = jrb_find_gen(tree, v2, compareLongInt);
    jrb_delete_node(node2);

    if (graph.type == UNDIRECTED)
      deleteEdge(graph, v2, v1);
  }
}

//-----------------------------------------

void updateWeight(Graph graph, Jval v1, Jval v2, double newWeight) {
  deleteEdge(graph, v1, v2);
  addEdge(graph, v1, v2, newWeight);
}

// ------------------------------------------------------------
// QUERY functions
// ------------------------------------------------------------

char *getVertex(Graph graph, Jval v) {
  JRB node = jrb_find_gen(graph.vertices, v, compareLongInt);

  if (node == NULL)
    return NULL;
  else
    return jval_s(node->val);
}

//-----------------------------------------

int getVertexId(Graph graph, char *name) {
  JRB node;

  jrb_traverse(node, graph.vertices) {
    if (strcmp(name, jval_s(node->val)) == 0)
      return jval_i(node->key);
  }

  return BOOL_NOT_FOUND;
}

//-----------------------------------------

double getEdgeValue(Graph graph, Jval v1, Jval v2) {
  JRB node, tree;

  node = jrb_find_gen(graph.edges, v1, compareLongInt);
  if (node == NULL)
    return INFINITIVE_VALUE;

  tree = (JRB) jval_v(node->val);
  node = jrb_find_gen(tree, v2, compareLongInt);
  if (node == NULL)
    return INFINITIVE_VALUE;
  else
    return jval_d(node->val);
}

//-----------------------------------------

int getVertexNum(Graph graph) {
  int counter = 0;
  JRB node;

  jrb_traverse(node, graph.vertices) counter++;

  return counter;
}

//-----------------------------------------

int getEdgeNum(Graph graph) {
  JRB v1, v2;
  int counter = 0;

  jrb_traverse(v1, graph.vertices) {
    jrb_traverse(v2, graph.vertices) {
      if (hasEdge(graph, v1->key, v2->key))
        counter++;
    }
  }

  if (graph.type == UNDIRECTED)
    return counter / 2;
  else
    return counter;
}

//-----------------------------------------

int indegree(Graph graph, Jval v, long *output) {
  if (getVertex(graph, v) == NULL) {
    printf("> Vertex %ld not found!\n", jval_l(v));
    return BOOL_NOT_FOUND;
  }

  JRB tree, node;
  int total = 0;

  jrb_traverse(node, graph.edges) {
    tree = (JRB) jval_v(node->val);

    if (jrb_find_gen(tree, v, compareLongInt)) {
      output[total] = jval_l(node->key);
      total++;
    }
  }

  return total;
}

//-----------------------------------------

int outdegree(Graph graph, Jval v, long *output) {
  if (getVertex(graph, v) == NULL) {
    printf("Vertex %ld not found!\n", jval_l(v));
    return BOOL_NOT_FOUND;
  }

  JRB tree, node;
  int total;

  node = jrb_find_gen(graph.edges, v, compareLongInt);
  if (node == NULL)
    return 0;

  tree = (JRB) jval_v(node->val);
  total = 0;
  jrb_traverse(node, tree) {
    output[total] = jval_l(node->key);
    total++;
  }

  return total;
}

// ------------------------------------------------------------
// CHECK functions: hasEdge,... Return boolean
// ------------------------------------------------------------

int hasEdge(Graph graph, Jval v1, Jval v2) {
  JRB node = jrb_find_gen(graph.edges, v1, compareLongInt);
  if (node == NULL)
    return BOOL_FALSE;

  JRB tree = (JRB) jval_v(node->val);
  if (jrb_find_gen(tree, v2, compareLongInt) == NULL)
    return BOOL_FALSE;
  else
    return BOOL_TRUE;
}

// ------------------------------------------------------------
// GRAPH TRAVESAL functions
// ------------------------------------------------------------

int DFS(Graph graph, long graph_size, long start, long stop, long *path) {
  int *visited;
  long *save;
  long u;

  visited = (int *) malloc((graph_size + 1) * sizeof(int));
  save = (long *) malloc((graph_size + 1) * sizeof(long));
  Dllist stack = new_dllist();

  dll_append(stack, new_jval_l(start));

  while (!dll_empty(stack)) {
    Dllist node = dll_last(stack);
    u = jval_l(node->val);
    printf("\n\nu = %ld, visited = %d", u, visited[u]);
    dll_delete_node(node);

    if (visited[u] != BOOL_TRUE) {
      visited[u] = BOOL_TRUE;
      if (u == stop)
        break;
      long output[200];
      int n = outdegree(graph, new_jval_l(u), output);

      printf(" | n = %d (", n);
      for (int i = 0; i < n; i++) {
        printf(" %ld ", output[i]);
      }
      printf(")\n");

      for (int i = 0; i < n; i++) {
        printf("i = %d | ", i);
        long v = output[i];
        printf("v = %ld, visited = %d", v, visited[v]);
        if (visited[v] != BOOL_TRUE) {
          save[v] = u;
          dll_append(stack, new_jval_l(v));
          printf(", Add save[%ld] = %ld", v, save[v]);
        }
        printf("\n");
      }
    }
  }
  printf("\nNow save path\n");

  int pathCount = 0;

  if (u != stop)
    return 0;
  else {
    long i = stop;
    path[pathCount++] = i;
    while (i != start) {
      i = save[i];
      path[pathCount++] = i;
    }
    reverseArray(path, pathCount);
  }

  // Deallocate memory
  free_dllist(stack);
  free(save);
  free(visited);

  return pathCount;
}

//-----------------------------------------

int connectedComponents(Graph graph, long graph_vertices_num, FILE *output_fptr) {
  int *visited;
  long u;
  long output[200];
  int counter = 0;
  JRB node;

  visited = (int *) malloc((graph_vertices_num + 1) * sizeof(int));
  Dllist stack = new_dllist();

  jrb_traverse(node, graph.vertices) {
    u = jval_l(node->key);

    if (visited[u] != BOOL_TRUE) {
      long *componentArr;
      int componentSize = 0;
      long edgeNum = 0;
      int maxDegree = -INFINITIVE_VALUE;
      int minDegree = INFINITIVE_VALUE;
      long maxDegreeNode, minDegreeNode;

      dll_append(stack, new_jval_l(u));

      while (!dll_empty(stack)) {
        Dllist last = dll_last(stack);

        u = jval_l(last->val);
        dll_delete_node(last);

        if (visited[u] != BOOL_TRUE) {
          visited[u] = BOOL_TRUE;
          componentSize++;

          int n = outdegree(graph, new_jval_l(u), output);
          int visitedEdges = 0;

          if (n > maxDegree) {
            maxDegree = n;
            maxDegreeNode = u;
          }
          if (n < minDegree) {
            minDegree = n;
            minDegreeNode = u;
          }

          for (int i = 0; i < n; i++) {
            long v = output[i];
            if (visited[v] != BOOL_TRUE)
              dll_append(stack, new_jval_l(v));
            else
              visitedEdges++;
          }

          edgeNum += n - visitedEdges;
        }
      }

      counter++;

      // Log component information
      fprintf(output_fptr, "=================================================\n");
      fprintf(output_fptr, "|| Component %-6d                            ||\n", counter);
      fprintf(output_fptr, "||---------------------------------------------||\n");
      fprintf(output_fptr, "|| %-30s %'12d ||\n", "> No. of vertices:", componentSize);
      fprintf(output_fptr, "|| %-30s %'12ld ||\n", "> No. of edges:", edgeNum);
      fprintf(output_fptr, "|| > Minimum degree (%12ld):%5s %'5d ||\n", minDegreeNode, "", minDegree);
      fprintf(output_fptr, "|| > Maximum degree (%12ld):%5s %'5d ||\n", maxDegreeNode, "", maxDegree);
      fprintf(output_fptr, "=================================================\n\n");
    }
  }

  // Deallocate memory
  free(visited);
  free_dllist(stack);

  return counter;
}
