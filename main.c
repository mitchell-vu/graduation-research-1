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

void printVertex(Jval v) { printf("%3ld ", jval_l(v)); }

int main(void) {
  setlocale(LC_NUMERIC, "en_US.utf-8");

  int currentOption;
  char *filename;
  clock_t tic, toc;

  do {
    system("clear");
    system("printf '\e[3J'");
    currentOption = printStateMenu();

    switch (currentOption) {
    case 0:
      filename = "examples.txt";
      break;
    case 1:
      filename = "roadNet-CA.txt";
      break;
    case 2:
      filename = "roadNet-PA.txt";
      break;
    case 3:
      filename = "roadNet-TX.txt";
      break;
    default:
      break;
    }

    // If valid option was chosen
    if (currentOption <= 4) {
      if (currentOption == 4) {
        printYellow("> Exiting...\n");
        break;
      }

      char outputFilename[200];
      sprintf(outputFilename, "out/%s", filename);

      FILE *outputFptr;
      outputFptr = fopen(outputFilename, "w+");

      if (outputFptr == NULL) {
        printf("Error opening file.\n");
        exit(1);
      }

      // ------------------------------------------------------------
      // Fetch graph from file
      printf("[ 25%%] ");
      char msg[200];
      sprintf(msg, "Building graph from file `data/%s`\n", filename);
      printGreen(msg);

      tic = clock();
      Graph roadGraph = createGraph(UNDIRECTED);
      fetchGraph(roadGraph, filename);
      toc = clock();
      printf("[ 25%%] Built graph in %.3fs\n", (double) (toc - tic) / CLOCKS_PER_SEC);

      // ------------------------------------------------------------
      // Calculate the number of connected components
      printf("[ 75%%] ");
      printGreen("Calculating the number of connected components\n");

      tic = clock();
      int componentNum = connectedComponents(roadGraph, outputFptr);
      toc = clock();
      printf("[ 75%%] Calculated the number of connected component"
             " in %.3fs\n",
             (double) (toc - tic) / CLOCKS_PER_SEC);

      // ------------------------------------------------------------
      // Log components specifications to file
      printf("[100%%] ");
      printGreen("Logging connected components specifications\n");
      int vertexNum = getVertexNum(roadGraph);
      printf("[100%%] Logged connected components specifications into `%s`\n", outputFilename);

      printf("\n> Number of vertices: %-'12d", vertexNum);
      printf("\n> Number of connected components: %-'12d\n", componentNum);

      // printf("\n> DFS: start from node 1 to 4:\n");
      // DFS(roadGraph, new_jval_l(1), new_jval_l(4), printVertex);

      // printf("\n> BFS: start from node 1 to 4:\n");
      // BFS(roadGraph, new_jval_l(1), new_jval_l(4), printVertex);

      // Free memory
      fclose(outputFptr);
      dropGraph(roadGraph);

      printWait();
    } else {
      printError("Invalid option!\n");
      printWait();
    }

  } while (currentOption != 4);

  return 0;
};
