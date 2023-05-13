#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

#include "lib/print/print.h"
#include "lib/fields/fields.h"
#include "lib/graph/graph.h"

void fetchGraph(Graph graph, char *filename)
{
    char *inputFilename = (char *)malloc(100 * sizeof(char));
    strcpy(inputFilename, "data/");
    strcat(inputFilename, filename);

    IS is = new_inputstruct(inputFilename);

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

    int currentOption;
    char *filename;
    clock_t tic, toc;

    do
    {
        system("clear");
        system("printf '\e[3J'");
        currentOption = printStateMenu();

        switch (currentOption)
        {
        case 0:
            filename = "roadNet-CA-md.txt";
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

        if (currentOption <= 4)
        {
            if (currentOption == 4)
            {
                printYellow("> Exiting...\n");
                break;
            }

            // ------------------------------------------------------------
            // Fetch graph from file
            printf("[ 25%%] ");
            char *msg = (char *)malloc(200 * sizeof(char));
            strcpy(msg, "Building graph from file `data/");
            strcat(msg, filename);
            strcat(msg, "`\n");
            printGreen(msg);
            free(msg);

            tic = clock();
            Graph roadGraph = createGraph(UNDIRECTED);
            fetchGraph(roadGraph, filename);
            toc = clock();
            printf("[ 25%%] Built graph in %.2fs\n", (double)(toc - tic) / CLOCKS_PER_SEC);

            // ------------------------------------------------------------
            // Calculate the number of connected components
            printf("[ 75%%] ");
            printGreen("Calculating the number of connected components\n");

            tic = clock();
            int vertexNum = getVertexNum(roadGraph);
            int componentCount = connectedComponents(roadGraph, (long)2000000, filename);
            toc = clock();
            printf("[ 75%%] Calculated the number of connected components in %.2fs\n", (double)(toc - tic) / CLOCKS_PER_SEC);

            // ------------------------------------------------------------
            // Log components specifications to file
            printf("[100%%] ");
            printGreen("Logging connected components specifications\n");
            printf("[100%%] Logged connected components specifications\n");

            // Free memory
            dropGraph(roadGraph);
            printWait();
        }
        else
        {
            printError("Invalid option!\n");
            printWait();
        }
    } while (currentOption != 4);

    return 0;
};
