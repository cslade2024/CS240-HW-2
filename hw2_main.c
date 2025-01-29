/*
 * Homework 2 main program
 * CS 240, Fall 2024
 * Purdue University
 */

#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "hw2.h"

#define MAX_FILENAME_SIZE (512)

/* Helper function declarations */

static int read_int(const char *);
static void read_line(const char *restrict, char *restrict, size_t);

/*
 * This function is used to run the functions implemented in hw2.c.
 * User input is used to determine which function to run what input
 * is passed to it. Upon completion, the function returns zero.
 */

int main() {
  while (1) {
    int choice = 0;
    printf(
      "\nSelect a function to test:\n"
      "    0) Exit\n"
      "    --\n"
      "    1) count_logins()\n"
      "    2) total_cpu_usage()\n"
      "    3) editor_wars()\n"
      "    4) blame_the_kids()\n"
      "    5) efficiency_score()\n"
      "    6) generate_report()\n"
    );
    choice = read_int("Enter your choice: ");
    putchar('\n');

    if (choice == 0) {
      break;
    }

    /* Variables to store input parameters in */

    int year = 0;
    int month = 0;
    int day = 0;
    int start_date = 0;
    int end_date = 0;
    char lab[MAX_NAME_SIZE] = {0};
    char input_file[MAX_FILENAME_SIZE] = {0};
    char output_file[MAX_NAME_SIZE] = {0};

    if ((choice >= 1) && (choice <= 6)) {
      read_line("Enter the input file name: ", input_file, sizeof(input_file));
    }

    if (choice == 6) {
      read_line("Enter the output file name: ", output_file, sizeof(output_file));
    }

    if ((choice == 1) || (choice == 2) || (choice == 5) || (choice == 6)) {
      read_line("Enter the lab name: ", lab, sizeof(lab));
    }

    if (choice == 1) {
      year = read_int("Enter year: ");
      month = read_int("Enter month: ");
      day = read_int("Enter day: ");
    }

    if ((choice == 2) || (choice == 6)) {
      start_date = read_int("Enter start date (as YYYYMMDD integer): ");
      end_date = read_int("Enter end date (as YYYYMMDD integer): ");
    }

    switch (choice) {
      case 1:    /* count_logins() */
        printf("count_logins(%s, %s, %d, %d, %d) returned %d\n",
               input_file, lab, year, month, day,
               count_logins(input_file, lab, year, month, day));
        break;

      case 2:    /* total_cpu_usage() */
        printf("total_cpu_usage(%s, %s, %d, %d) returned %lf\n",
               input_file, lab, start_date, end_date,
               total_cpu_usage(input_file, lab, start_date, end_date));
        break;

      case 3:    /* editor_wars() */
        printf("editor_wars(%s) returned %lf\n",
               input_file, editor_wars(input_file));
        break;

      case 4:    /* blame_the_kids() */
        printf("blame_the_kids(%s) returned %d\n",
               input_file, blame_the_kids(input_file));
        break;

      case 5: /* efficiency_score() */
        printf("efficiency_score(%s, %s) returned %lf\n",
               input_file, lab, efficiency_score(input_file, lab));
        break;

      case 6: /* generate_report() */
        do {
        } while (0);
        int result = generate_report(input_file, output_file, lab, start_date,
                                     end_date);
        printf("generate_report(%s, %s, %s, %d, %d) returned %d\n",
               input_file, output_file, lab, start_date, end_date, result);
        printf("Note: you should inspect the contents of the output file.\n");
        if (result < 0) {
          printf("Consider checking the output file.\n");
        }
        break;

      default:
        printf("\nSelect one of the given options.\n");
        break;
    }
  }

  return 0;
} /* main() */

/*
 * Prints the prompt, then reads a line containing a single integer from
 * standard input. Repeats the prompt/input stage until a valid input is read.
 * Returns the integer read.
 */

int read_int(const char *prompt) {
  do {
    int num = 0;
    fputs(prompt, stdout);
    if ((scanf("%d", &num) == 1) && (getchar() == '\n')) {
      return num;
    }
    else {
      while (getchar() != '\n') {
      }
      printf("Invalid choice. Try again.\n");
    }
  } while (1);
} /* read_int() */

/*
 * Prints the given prompt, then reads a line from standard input, and stores it
 * in the given buffer. Truncates the stored string to the given size, including
 * the NUL terminator.
 */

void read_line(const char *restrict prompt, char *restrict buf, size_t size) {
  fputs(prompt, stdout);
  assert(fgets(buf, size - 1, stdin) != NULL);
  size_t len = strlen(buf);
  if (buf[len - 1] == '\n') {
    buf[len - 1] = '\0';
  }
  else {
    while (getchar() != '\n') {
    }
  }
} /* read_line() */
