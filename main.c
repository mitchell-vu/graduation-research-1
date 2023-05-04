#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

#include "lib/print/print.h"
#include "lib/fields/fields.h"
#include "lib/graph/graph.h"

void fetchGraph(Graph graph, int option)
{
    char *filename;
    switch (option)
    {
    case 1:
        filename = "data/roadNet-CA-min.txt";
        break;
    case 2:
        filename = "data/roadNet-PA.txt";
        break;
    case 3:
        filename = "data/roadNet-TX.txt";
        break;
    default:
        break;
    }

    IS is = new_inputstruct(filename);

    if (is == NULL)
    {
        char *errorMsg = malloc(100);
        strcpy(errorMsg, "Cannot read data from file `");
        strcat(errorMsg, filename);
        strcat(errorMsg, "`\n");
        printError(errorMsg);
        free(errorMsg);
        exit(1);
    }

    while (get_line(is) >= 0)
    {
        if (is->NF == 1 || is->fields[0][0] == '#')
            continue;
        else
        {
            addEdge(graph, atoi(is->fields[0]), atoi(is->fields[1]), 1);
        }
    }
    jettison_inputstruct(is);
}

int main(void)
{
    setlocale(LC_NUMERIC, "en_US.utf-8");

    int stateOption;
    do
    {
        // system("clear");
        // system("printf '\e[3J'");

        stateOption = printStateMenu();
        if (stateOption <= 4)
        {
            if (stateOption == 4)
            {
                printYellow("> Exiting...\n");
                break;
            }

            clock_t tic, toc;
            Graph roadGraph = createGraph(DIRECTED);

            printf("> Load Graph...\n");

            tic = clock();
            fetchGraph(roadGraph, stateOption);
            toc = clock();
            printf("> Graph has been fetched successfully!  (%.2fs)\n", (double)(toc - tic) / CLOCKS_PER_SEC);

            printf("\nGraph specs:\n");

            tic = clock();
            int vertexNum = getVertexNum(roadGraph);
            toc = clock();
            printf("%-25s %'12d  (%.2fs)\n", "No. of vertices:", vertexNum, (double)(toc - tic) / CLOCKS_PER_SEC);

            tic = clock();
            int egdeNum = getEdgeNum(roadGraph);
            toc = clock();
            printf("%-25s %'12d  (%.2fs)\n", "No. of edges:", egdeNum, (double)(toc - tic) / CLOCKS_PER_SEC);

            printWait();
            dropGraph(roadGraph);
        }
        else
        {
            printError("Invalid option!\n");
            printWait();
        }
    } while (stateOption != 4);

    return 0;
};
