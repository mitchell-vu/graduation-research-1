#include <locale.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "lib/fields/fields.h"
#include "lib/graph/graph.h"
#include "lib/print/print.h"

void fetchGraph(Graph graph, char *filename) {
  char inputFilename[200];
  sprintf(inputFilename, "data/%s", filename);

  IS is = new_inputstruct(inputFilename);

  if (is == NULL) {
    char errorMsg[200];
    sprintf(errorMsg, "Cannot read data from file `%s`\n", filename);
    printError(errorMsg);

    jettison_inputstruct(is);
    exit(1);
  }

  while (get_line(is) >= 0) {
    // Skip comments
    if (is->NF == 1 || is->fields[0][0] == '#')
      continue;
    else
      addEdge(graph, new_jval_l(atol(is->fields[0])), new_jval_l(atol(is->fields[1])), 1);
  }

  jettison_inputstruct(is);
}

void printVertex(Jval v) { printf("Visiting [%ld]...\n", jval_l(v)); }

int main(void) {
  setlocale(LC_NUMERIC, "en_US.utf-8");

  int currentOption, counter;
  char filename[100];
  char outputFilename[200];
  long startVertex, endVertex;
  clock_t tic, toc;

  system("clear");
  system("printf '\e[3J'");

  printf("> Enter input file name in `data/` folder: ");
  scanf("%s", filename);

  printf("> ");
  char msg[200];
  sprintf(msg, "Building graph from file `data/%s`\n", filename);
  printGreen(msg);

  tic = clock();
  Graph roadGraph = createGraph(UNDIRECTED);
  fetchGraph(roadGraph, filename);
  toc = clock();
  printf("> Built graph in %.3fs\n", (double) (toc - tic) / CLOCKS_PER_SEC);

  printWait();

  do {
    // system("clear");
    // system("printf '\e[3J'");
    currentOption = printActionsMenu();

    switch (currentOption) {
    // ------------------------------------------------------------
    // Display graph specifications
    case 1:
      printf("\n> Graph Specifications:\n");
      printGreen("* Name: ");
      printf("%s\n", filename);
      printGreen("* Number of vertices: ");
      printf("%-'12d\n", getVertexNum(roadGraph));
      printGreen("* Type: ");
      printf("%s\n", roadGraph.type == DIRECTED ? "Directed" : "Undirected");
      printWait();
      break;

    // ------------------------------------------------------------
    // Depth First Search
    case 2:
      printf("> Enter starting vertex: ");
      scanf("%ld", &startVertex);
      printf("> Enter ending vertex: ");
      scanf("%ld", &endVertex);

      tic = clock();
      printf("> Depth First Search from [%ld] -> [%ld]:\n", startVertex, endVertex);
      counter = DFS(roadGraph, new_jval_l(startVertex), new_jval_l(endVertex), printVertex);
      toc = clock();

      printGreen("\n\n> Depth First Search from ");
      printf("[%ld]", startVertex);
      printGreen(" -> ");
      printf("[%ld]", endVertex);
      printGreen(" finished in ");
      printf("%.3fs\n", (double) (toc - tic) / CLOCKS_PER_SEC);
      printGreen("> Traversed vertices number: ");
      printf("%-'12d\n", counter);

      printWait();
      break;

    // ------------------------------------------------------------
    // Breadth First Search
    case 3:
      printf("> Enter starting vertex: ");
      scanf("%ld", &startVertex);
      printf("> Enter ending vertex: ");
      scanf("%ld", &endVertex);

      tic = clock();
      printf("> Breadth First Search from [%ld] -> [%ld]:\n", startVertex, endVertex);
      counter = BFS(roadGraph, new_jval_l(startVertex), new_jval_l(endVertex), printVertex);
      toc = clock();

      printGreen("\n\n> Breadth First Search from ");
      printf("[%ld]", startVertex);
      printGreen(" -> ");
      printf("[%ld]", endVertex);
      printGreen(" finished in ");
      printf("%.3fs\n", (double) (toc - tic) / CLOCKS_PER_SEC);
      printGreen("> Traversed vertices number: ");
      printf("%-'12d\n", counter);

      printWait();
      break;

    // ------------------------------------------------------------
    // Calculate the number of connected components
    case 4:
      sprintf(outputFilename, "out/%s", filename);

      FILE *outputFptr;
      outputFptr = fopen(outputFilename, "w+");

      if (outputFptr == NULL) {
        printf("Error opening file.\n");
        break;
      }

      printGreen("> Calculating the number of connected components\n");

      tic = clock();
      int componentNum = connectedComponents(roadGraph, outputFptr);
      toc = clock();

      printGreen("> Calculated the number of connected component in ");
      printf("%.3fs\n", (double) (toc - tic) / CLOCKS_PER_SEC);

      printf("> Logged connected components specifications into `%s`\n", outputFilename);
      printf("\n> Number of connected components: %-'12d\n", componentNum);

      fclose(outputFptr);
      printWait();
      break;

    // ------------------------------------------------------------
    // Exit the program
    case 5:
      printYellow("> Exiting...\n");
      break;

    // ------------------------------------------------------------
    // Invalid options
    default:
      printError("Invalid option!\n");
      break;
    }
  } while (currentOption != 5);

  dropGraph(roadGraph);
  return 0;
};
