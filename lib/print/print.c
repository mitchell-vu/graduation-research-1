#include "print.h"

extern void printGreen(char *string) { printf(COLOR_GREEN "%s" COLOR_RESET, string); }

extern void printYellow(char *string) { printf(COLOR_YELLOW "%s" COLOR_RESET, string); }

extern void printError(char *message) { printf(COLOR_RED "(!) %s" COLOR_RESET, message); }

void clear_buffer() {
  int ch;
  while ((ch = getchar()) != '\n' && ch != EOF) {
    continue;
  }
}

extern void printWait(void) {
  int enter = 0;

  printYellow("\n> Press ENTER to continue...\n");
  while (enter != '\r' && enter != '\n') {
    enter = getchar();
  }
}

int printActionsMenu(void) {
  char menu[][100] = {
      "Graph Specifications", "Depth First Search", "Breath First Search", "Count Connected Components", "Exit",
  };
  int choice;

  // Each row of menu has 58 characters
  printGreen("\n--------------------- ACTIONS MENU -----------------------\n");
  printf(COLOR_GREEN "| %-54s |\n", "");

  for (int i = 0; i < 5; i++) {
    printf("| " COLOR_RESET "%-50s [%d]" COLOR_GREEN " |\n", menu[i], i + 1);
  }

  printf("| %-54s |\n", "");
  printGreen("----------------------------------------------------------\n\n");

  printf("> Choose an action: ");
  scanf("%d", &choice);
  clear_buffer();

  printf("\n");
  return choice;
};