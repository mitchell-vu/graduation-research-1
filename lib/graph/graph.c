#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "graph.h"

//***************************************************************
//** CREATE - DESTROY functions *********************************

Graph createGraph(int type)
{
  Graph g;
  g.vertices = make_jrb();
  g.edges = make_jrb();
  g.type = type;
  return g;
}

//-----------------------------------------

void dropGraph(Graph graph)
{
  JRB node;

  // Free edges tree
  jrb_traverse(node, graph.edges)
  {
    jrb_free_tree((JRB)jval_v(node->val));
  }
  jrb_free_tree(graph.edges);

  // Free vertices tree
  jrb_free_tree(graph.vertices);
}

//***************************************************************
//** ADD - DELETE functions *************************************

int compareLongInt(Jval j1, Jval j2)
{
  long i1 = j1.l;
  long i2 = j2.l;
  if (i1 > i2)
    return 1;
  if (i1 < i2)
    return -1;
  return 0;
}

void addVertex(Graph graph, long id, char *name)
{
  JRB node = jrb_find_gen(graph.vertices, new_jval_l(id), compareLongInt);
  if (node == NULL)
  {
    // char *strdup(char *s) returns a pointer to the duplicated string s.
    jrb_insert_gen(graph.vertices, new_jval_l(id), new_jval_s(strdup(name)), compareLongInt);
    // printf("Added vertex (%d)\n", id);
  }
}

//-----------------------------------------

void addEdge(Graph graph, long v1, long v2, double weight)
{
  if (getVertex(graph, v1) == NULL)
    addVertex(graph, v1, "");

  if (getVertex(graph, v2) == NULL)
    addVertex(graph, v2, "");

  JRB node, tree;
  int allowToAdd;

  if (graph.type == DIRECTED)
  {
    if (getEdgeValue(graph, v1, v2) == INFINITIVE_VALUE)
      allowToAdd = BOOL_TRUE;
    else
      allowToAdd = BOOL_FALSE;
  }
  else
  {
    if (getEdgeValue(graph, v1, v2) == INFINITIVE_VALUE || getEdgeValue(graph, v2, v1) == INFINITIVE_VALUE)
      allowToAdd = BOOL_TRUE;
    else
      allowToAdd = BOOL_FALSE;
  }

  if (allowToAdd == BOOL_TRUE)
  {
    node = jrb_find_gen(graph.edges, new_jval_l(v1), compareLongInt);
    if (node == NULL) // Check xem có sẵn cây trong node chưa
    {
      tree = make_jrb();
      jrb_insert_gen(graph.edges, new_jval_l(v1), new_jval_v(tree), compareLongInt);
    }
    else
      tree = (JRB)jval_v(node->val);

    jrb_insert_gen(tree, new_jval_l(v2), new_jval_d(weight), compareLongInt);

    if (graph.type == UNDIRECTED)
      addEdge(graph, v2, v1, weight);
  }
}

//-----------------------------------------

void deleteEdge(Graph graph, int v1, int v2)
{
  JRB node1;
  JRB node;

  if (getEdgeValue(graph, v1, v2) != INFINITIVE_VALUE)
  {
    node = jrb_find_int(graph.edges, v1);
    JRB tree = (JRB)jval_v(node->val);
    node1 = jrb_find_int(tree, v2);
    jrb_delete_node(node1);

    if (graph.type == UNDIRECTED)
    {
      deleteEdge(graph, v2, v1);
    }
  }
}

//-----------------------------------------

void deleteVertex(Graph graph, int v)
{
  JRB node = jrb_find_int(graph.vertices, v);
  jrb_delete_node(node);
  JRB node1 = jrb_find_int(graph.edges, v);
  JRB tree = (JRB)jval_v(node1->val);
  jrb_free_tree(tree);
  jrb_delete_node(node1);
}

//-----------------------------------------

void updateVertex(Graph graph, int v, char *newName)
{
  deleteVertex(graph, v);
  addVertex(graph, v, newName);
}

//-----------------------------------------

void updateWeight(Graph graph, int v1, int v2, double newWeight)
{
  deleteEdge(graph, v1, v2);
  addEdge(graph, v1, v2, newWeight);
}

//***************************************************************
//** QUERY functions ********************************************

char *getVertex(Graph graph, long id)
{
  JRB node = jrb_find_gen(graph.vertices, new_jval_l(id), compareLongInt);
  if (node == NULL)
  {
    return NULL;
  }
  else
    return jval_s(node->val);
}

//-----------------------------------------

int getVertexId(Graph graph, char *name)
{
  JRB node;
  jrb_traverse(node, graph.vertices)
  {
    // printf("(%s - %s)", name, jval_s(node->val));
    if (strcmp(name, jval_s(node->val)) == 0)
    {
      return jval_i(node->key);
    }
  }

  return -1;
}

//-----------------------------------------

double getEdgeValue(Graph graph, int v1, int v2)
{
  JRB node, tree;

  node = jrb_find_int(graph.edges, v1);
  if (node == NULL)
  {
    // printf("Vertex %d not found!\n", v1);
    return INFINITIVE_VALUE;
  }

  tree = (JRB)jval_v(node->val);
  node = jrb_find_int(tree, v2);
  if (node == NULL)
    return INFINITIVE_VALUE;
  else
    return jval_d(node->val);
}

//-----------------------------------------

int getVertexNum(Graph graph)
{
  int counter = 0;
  JRB node;

  jrb_traverse(node, graph.vertices)
      counter++;

  return counter;
}

//-----------------------------------------

int getEdgeNum(Graph graph)
{
  JRB v1, v2;
  int counter = 0;

  jrb_traverse(v1, graph.vertices)
  {
    jrb_traverse(v2, graph.vertices)
    {
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

int indegree(Graph graph, int v, int *output)
{
  if (getVertex(graph, v) == NULL)
  {
    printf("Vertex %d not found!\n", v);
    return -1;
  }

  JRB tree, node;
  int total = 0;

  jrb_traverse(node, graph.edges)
  {
    tree = (JRB)jval_v(node->val);
    if (jrb_find_int(tree, v))
    {
      output[total] = jval_i(node->key);
      total++;
    }
  }

  return total;
}

//-----------------------------------------

int outdegree(Graph graph, long v, long *output)
{
  if (getVertex(graph, v) == NULL)
  {
    printf("Vertex %ld not found!\n", v);
    return -1;
  }

  JRB tree, node;
  int total;

  node = jrb_find_gen(graph.edges, new_jval_l(v), compareLongInt);
  if (node == NULL)
    return 0;

  tree = (JRB)jval_v(node->val);
  total = 0;
  jrb_traverse(node, tree)
  {
    output[total] = jval_l(node->key);
    total++;
  }

  return total;
}

//***************************************************************
//** CHECK functions ********************************************

int hasEdge(Graph graph, Jval v1, Jval v2)
{
  JRB node = jrb_find_gen(graph.edges, v1, compareLongInt);
  if (node == NULL)
    return BOOL_FALSE;

  JRB tree = (JRB)jval_v(node->val);
  if (jrb_find_gen(tree, v2, compareLongInt) == NULL)
    return BOOL_FALSE;
  else
    return BOOL_TRUE;
}

//***************************************************************
//** GRAPH TRAVESAL functions ***********************************

void swapArray(long *arr, int cnt)
{
  for (int i = 0; i < cnt / 2; i++)
  {
    int c = arr[i];
    arr[i] = arr[cnt - i - 1];
    arr[cnt - i - 1] = c;
  }
}

int DFS(Graph graph, long graph_size, long start, long stop, long *path)
{
  int *visited;
  long *save;
  long u;

  visited = (int *)malloc((graph_size + 1) * sizeof(int));
  save = (long *)malloc((graph_size + 1) * sizeof(long));
  Dllist stack = new_dllist();

  dll_append(stack, new_jval_l(start));

  while (!dll_empty(stack))
  {
    Dllist node = dll_last(stack);
    u = jval_l(node->val);
    printf("\n\nu = %ld, visited = %d", u, visited[u]);
    dll_delete_node(node);

    if (visited[u] != BOOL_TRUE)
    {
      visited[u] = BOOL_TRUE;
      if (u == stop)
        break;
      long output[200];
      int n = outdegree(graph, u, output);

      printf(" | n = %d (", n);
      for (int i = 0; i < n; i++)
      {
        printf(" %ld ", output[i]);
      }
      printf(")\n");

      for (int i = 0; i < n; i++)
      {
        printf("i = %d | ", i);
        long v = output[i];
        printf("v = %ld, visited = %d", v, visited[v]);
        if (visited[v] != BOOL_TRUE)
        {
          save[v] = u;
          dll_append(stack, new_jval_l(v));
          printf(", Add save[%ld] = %ld", v, save[v]);
        }
        printf("\n");
      }
    }
  }
  printf("\nNow save path\n");

  int cnt = 0;

  if (u != stop)
    return 0;
  else
  {
    long i = stop;
    path[cnt++] = i;
    while (i != start)
    {
      i = save[i];
      path[cnt++] = i;
    }
    swapArray(path, cnt);
  }

  // Deallocate memory
  free_dllist(stack);
  free(save);
  free(visited);

  return cnt;
}

int connectedComponents(Graph graph, long graph_vertices_num, char *filename)
{
  int *visited;
  long u;
  int counter = 0;
  JRB node;

  char *outputFilename = (char *)malloc(100 * sizeof(char));
  strcpy(outputFilename, "out/");
  strcat(outputFilename, filename);

  FILE *fptr;
  fptr = fopen(outputFilename, "w+");

  if (fptr == NULL)
  {
    printf("Error opening file.\n");
    exit(1);
  }

  visited = (int *)malloc((graph_vertices_num + 1) * sizeof(int));
  Dllist stack = new_dllist();

  jrb_traverse(node, graph.vertices)
  {
    u = jval_l(node->key);

    if (visited[u] != BOOL_TRUE)
    {
      long *componentArr;
      int componentSize = 0;
      long edgeNum = 0;
      int maxDegree = -INFINITIVE_VALUE;
      int minDegree = INFINITIVE_VALUE;
      long maxDegreeNode, minDegreeNode;

      componentArr = (long *)malloc((graph_vertices_num + 1) * sizeof(long));
      dll_append(stack, new_jval_l(u));

      while (!dll_empty(stack))
      {
        Dllist node = dll_last(stack);

        u = jval_l(node->val);
        dll_delete_node(node);

        if (visited[u] != BOOL_TRUE)
        {
          visited[u] = BOOL_TRUE;
          componentArr[componentSize++] = u;

          long output[200];
          int n = outdegree(graph, u, output);
          int visitedEdges = 0;

          if (n > maxDegree)
          {
            maxDegree = n;
            maxDegreeNode = u;
          }
          if (n < minDegree)
          {
            minDegree = n;
            minDegreeNode = u;
          }

          for (int i = 0; i < n; i++)
          {
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
      fprintf(fptr, "=================================================\n");
      fprintf(fptr, "|| Component %-6d                            ||\n", counter);
      fprintf(fptr, "||---------------------------------------------||\n");
      fprintf(fptr, "|| %-30s %'12d ||\n", "> No. of vertices:", componentSize);
      fprintf(fptr, "|| %-30s %'12ld ||\n", "> No. of edges:", edgeNum);
      fprintf(fptr, "|| > Minimum degree (%12ld):%5s %'5d ||\n", minDegreeNode, "", minDegree);
      fprintf(fptr, "|| > Maximum degree (%12ld):%5s %'5d ||\n", maxDegreeNode, "", maxDegree);
      fprintf(fptr, "=================================================\n\n");

      free(componentArr);
    }
  }

  free(outputFilename);
  fclose(fptr);

  // Deallocate memory
  free(visited);
  free_dllist(stack);

  return counter;
}
