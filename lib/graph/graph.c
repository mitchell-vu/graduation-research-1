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

void addVertex(Graph graph, int id, char *name)
{
  JRB node = jrb_find_int(graph.vertices, id);
  if (node == NULL)
  {
    // char *strdup(char *s) returns a pointer to the duplicated string s.
    jrb_insert_int(graph.vertices, id, new_jval_s(strdup(name)));
    // printf("Added vertex (%d)\n", id);
  }
}

//-----------------------------------------

void addEdge(Graph graph, int v1, int v2, double weight)
{
  if (getVertex(graph, v1) == NULL)
  {
    addVertex(graph, v1, "");
  }

  if (getVertex(graph, v2) == NULL)
  {
    addVertex(graph, v2, "");
  }

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

    node = jrb_find_int(graph.edges, v1);
    if (node == NULL) // Check xem có sẵn cây trong node chưa
    {
      tree = make_jrb();
      jrb_insert_int(graph.edges, v1, new_jval_v(tree));
    }
    else
    {
      tree = (JRB)jval_v(node->val);
    }

    jrb_insert_int(tree, v2, new_jval_d(weight));
    // printf("Added edge (%d) ---%.2f---> (%d)\n", v1, weight, v2);

    if (graph.type == UNDIRECTED)
    {
      addEdge(graph, v2, v1, weight);
    }
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

void *getVertex(Graph graph, int id)
{
  JRB node = jrb_find_int(graph.vertices, id);
  if (node == NULL)
  {
    return NULL;
  }
  else
    return jval_s(node->val);
}

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
      if (hasEdge(graph, jval_i(v1->key), jval_i(v2->key)))
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

int outdegree(Graph graph, int v, int *output)
{
  if (getVertex(graph, v) == NULL)
  {
    printf("Vertex %d not found!\n", v);
    return -1;
  }

  JRB tree, node;
  int total;

  node = jrb_find_int(graph.edges, v);
  if (node == NULL)
    return 0;

  tree = (JRB)jval_v(node->val);
  total = 0;
  jrb_traverse(node, tree)
  {
    output[total] = jval_i(node->key);
    total++;
  }

  return total;
}

//***************************************************************
//** CHECK functions ********************************************

int hasEdge(Graph graph, int v1, int v2)
{
  JRB node = jrb_find_int(graph.edges, v1);
  if (node == NULL)
    return BOOL_FALSE;

  JRB tree = (JRB)jval_v(node->val);
  if (jrb_find_int(tree, v2) == NULL)
    return BOOL_FALSE;
  else
    return BOOL_TRUE;
}

//-----------------------------------------

int DAG(Graph graph)
{
  int start, notCycles;
  JRB node;

  jrb_traverse(node, graph.vertices)
  {
    start = jval_i(node->key);

    notCycles = DFS_DAG(graph, start);
    if (notCycles == BOOL_FALSE)
      return BOOL_FALSE;
  }

  return BOOL_TRUE;
}

//-----------------------------------------

int DFS_DAG(Graph graph, int start)
{
  // Dùng stack -> dllist chỉ dùng 2 functions:
  // dll_append( Dllist l, Jval val )                 -->   push()
  // dll_delete_node( dll_last(Dllist n), Jval val )  -->   pop()

  int visited[1000] = {};
  int output[100];
  Dllist node, stack;

  stack = new_dllist();                 // stack init
  dll_append(stack, new_jval_i(start)); // stack.push(start)

  while (!dll_empty(stack))
  {
    node = dll_last(stack);
    int u = jval_i(node->val); // lấy val của last
    dll_delete_node(node);     // stack.pop(last)

    if (!visited[u])
    {
      visited[u] = BOOL_TRUE;

      int n = outdegree(graph, u, output);
      for (int i = 0; i < n; i++)
      {
        int v = output[i];

        if (v == start)
          return BOOL_FALSE;

        if (!visited[v])
          dll_append(stack, new_jval_i(v)); // stack.push()
      }
    }
  }

  return BOOL_TRUE;
}

//***************************************************************
//** GRAPH TRAVESAL functions ***********************************

void swapArray(int arr[], int cnt)
{
  for (int i = 0; i < cnt / 2; i++)
  {
    int c = arr[i];
    arr[i] = arr[cnt - i - 1];
    arr[cnt - i - 1] = c;
  }
}

int topologicalSort(Graph graph, int *output)
{
  // Dùng stack -> dllist chỉ dùng 2 functions:
  // dll_append( Dllist l, Jval val )                 -->   enqueue()
  // dll_delete_node( dll_first(Dllist n), Jval val ) -->   dequeue()

  int indeg[1000], tmp[100];
  Dllist queue, node;
  queue = new_dllist();

  JRB vertex;
  jrb_traverse(vertex, graph.vertices)
  {
    int u = jval_i(vertex->key);
    indeg[u] = indegree(graph, u, tmp);

    if (indeg[u] == 0)                  // Searching for Source nodes
      dll_append(queue, new_jval_i(u)); // enqueue()
  }

  int total = 0;
  while (!dll_empty(queue))
  {
    node = dll_first(queue);
    int u = jval_i(node->val);
    dll_delete_node(node); // dequeue()
    output[total++] = u;

    int m = outdegree(graph, u, tmp); // Xoá các nodes đi ra
                                      // từ source node vừa xoá
    for (int i = 0; i < m; i++)
    {
      int v = tmp[i];
      indeg[v]--;
      if (indeg[v] == 0)                  // Put new Source node into queue
        dll_append(queue, new_jval_i(v)); // enqueue()
    }
  }
  swapArray(output, total);
  return total;
}

//-----------------------------------------

int DFS(Graph graph, int start, int stop, int *path)
{
  int visited[1000] = {};
  int output[100];
  int cnt = 0, u;
  int save[1000];
  Dllist stack = new_dllist();

  dll_append(stack, new_jval_i(start));

  while (!dll_empty(stack))
  {
    Dllist node = dll_last(stack);
    u = jval_i(node->val);
    dll_delete_node(node);

    if (!visited[u])
    {
      visited[u] = 1;
      if (u == stop)
        break;
      int n = outdegree(graph, u, output);
      for (int i = 0; i < n; i++)
      {
        int v = output[i];
        if (!visited[v])
        {
          save[v] = u;
          dll_append(stack, new_jval_i(v));
        }
      }
    }
  }
  if (u != stop)
    return 0;
  else
  {
    int i = stop;
    path[cnt++] = i;
    while (i != start)
    {
      i = save[i];
      path[cnt++] = i;
    }
    swapArray(path, cnt);
  }
  return cnt;
}

//-----------------------------------------

int BFS(Graph graph, int start, int stop, int *path)
{
  JRB dist = make_jrb();
  Dllist node;

  Dllist queue = new_dllist();
  dll_append(queue, new_jval_i(start));

  Dllist trace = new_dllist();
  dll_append(trace, new_jval_i(start));
  jrb_insert_int(dist, start, new_jval_v(trace));

  int u;
  while (!dll_empty(queue))
  {
    node = dll_first(queue);
    u = jval_i(node->val);
    dll_delete_node(node);

    Dllist u_find_path = jval_v(jrb_find_int(dist, u)->val);
    if (u == stop)
    {
      int cnt = 0;
      Dllist tmp;
      dll_traverse(tmp, u_find_path)
      {
        cnt++;
        if (path != NULL)
          path[cnt - 1] = jval_i(tmp->val);
      }
      return cnt;
    }

    int output[1000];
    int num = outdegree(graph, u, output);

    for (int i = 0; i < num; ++i)
    {
      JRB v_find = jrb_find_int(dist, output[i]);
      if (v_find == NULL)
      {
        Dllist current_path = new_dllist();
        Dllist node;
        dll_traverse(node, u_find_path)
            dll_append(current_path, node->val);

        dll_append(current_path, new_jval_i(output[i]));
        jrb_insert_int(dist, output[i], new_jval_v(current_path));
        dll_append(queue, new_jval_i(output[i]));
      }
    }
  }

  return 0;
}

//-----------------------------------------

double shortestPath(Graph graph, int start, int stop, int *path, int *numVertices)
{
  double distance[1000];
  int previous[1000], u, visit[1000];

  for (int i = 0; i < 1000; i++)
  {
    distance[i] = INFINITIVE_VALUE;
    visit[i] = 0;
    previous[i] = 0;
  }
  distance[start] = 0;
  previous[start] = start;
  visit[start] = 1;

  Dllist ptr, queue, node;
  queue = new_dllist();
  dll_append(queue, new_jval_i(start));

  while (!dll_empty(queue))
  {
    double min = INFINITIVE_VALUE;
    dll_traverse(ptr, queue)
    {
      u = jval_i(ptr->val);
      if (min > distance[u])
      {
        min = distance[u];
        node = ptr;
      }
    }

    u = jval_i(node->val);
    visit[u] = 1;
    dll_delete_node(node);
    if (u == stop)
      break;

    int output[1000];
    int n = outdegree(graph, u, output);

    for (int i = 0; i < n; i++)
    {
      int v = output[i];
      double w = getEdgeValue(graph, u, v);
      if (distance[v] > distance[u] + w)
      {
        distance[v] = distance[u] + w;
        previous[v] = u;
      }
      if (visit[v] == 0)
      {
        dll_append(queue, new_jval_i(v));
      }
    }
  }

  double distance_s_t = distance[stop];
  int count = 0;
  if (distance[stop] != INFINITIVE_VALUE)
  {
    path[count++] = stop;
    while (stop != start)
    {
      stop = previous[stop];
      path[count++] = stop;
    }
    *numVertices = count;
  }
  swapArray(path, count);

  return distance_s_t;
}