/*
 * Homework 2
 * Christian Slade
 * CS 240, Spring 2025
 * Purdue University
 */

/* Write your code here */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_LINE_SIZE 1024
#define SUCCESS 0
#define FILE_READ_ERR -1
#define FILE_WRITE_ERR -2
#define NO_DATA -3
#define BAD_DATE -4
#define BAD_RECORD -5

int count_logins(char *input_file, char *lab, int year, int month, int day)
{
  FILE *file = fopen(input_file, "r");
  if (file == NULL)
  {
    fprintf(stderr, "Error opening file\n");
    return -1;
  }

  char line[MAX_LINE_SIZE];
  int login_count = 0;
  int file_year, file_month, file_day;
  char file_lab[50];

  while (fscanf(file, "%d-%d-%d,\"%49[^\"]\",%*[^,],%*[^,],%*[^,],%*[^,],%*s\n", &file_year, &file_month, &file_day, file_lab) != EOF)
  {
    if (file_year == year && file_month == month && file_day == day && strcmp(file_lab, lab) == 0)
    {
      login_count++;
    }
  }

  fclose(file);
  return login_count;
}

double total_cpu_usage(char *input_file, char *lab, int start_date, int end_date) {
    FILE *file = fopen(input_file, "r");
    if (file == NULL) {
        fprintf(stderr, "Error opening file\n");
        return FILE_READ_ERR;
    }

    char line[MAX_LINE_SIZE];
    double total_cpu_minutes = 0.0;
    int file_year, file_month, file_day;
    char file_lab[50];
    double cpu_usage;
    int date;

    while (fgets(line, sizeof(line), file) != NULL) {
        if (sscanf(line, "%d-%d-%d,\"%49[^\"]\",%*[^,],%*[^,],%*[^,],%*[^,],%lf\n", &file_year, &file_month, &file_day, file_lab, &cpu_usage) != 5) {
            fclose(file);
            return BAD_DATE;
        }

        date = file_year * 10000 + file_month * 100 + file_day;
        if (date >= start_date && date <= end_date && strcmp(file_lab, lab) == 0) {
            total_cpu_minutes += cpu_usage;
        }
    }

    fclose(file);
    return total_cpu_minutes / 60.0; // Convert minutes to hours
}

double editor_wars(char *input_file)
{
  FILE *file = fopen(input_file, "r");
  if (file == NULL)
  {
    fprintf(stderr, "Error opening file\n");
    return FILE_READ_ERR;
  }

  char line[MAX_LINE_SIZE];
  double vi_popularity = 0.0;
  double emacs_popularity = 0.0;
  double session_duration;
  int processes_run;
  char top_program[50];

  while (fgets(line, sizeof(line), file) != NULL)
  {
    if (sscanf(line, "%*[^,],%*[^,],%*[^,],%lf,%d,\"%49[^\"]\",%*s\n", &session_duration, &processes_run, top_program) != 3)
    {
      fclose(file);
      return BAD_DATE;
    }

    if (strstr(top_program, "vi") != NULL)
    {
      vi_popularity += session_duration / processes_run;
    }
    else if (strstr(top_program, "macs") != NULL)
    {
      emacs_popularity += session_duration / processes_run;
    }
  }

  fclose(file);
  return vi_popularity - emacs_popularity;
}

int blame_the_kids(char *input_file)
{
  FILE *file = fopen(input_file, "r");
  if (file == NULL)
  {
    fprintf(stderr, "Error opening file\n");
    return FILE_READ_ERR;
  }

  char line[MAX_LINE_SIZE];
  char username[50];
  double cpu_usage;
  double max_cpu_usage = 0.0;
  int line_number = 0;
  int max_cpu_line_number = 0;

  while (fgets(line, sizeof(line), file) != NULL)
  {
    line_number++;
    if (sscanf(line, "%*[^,],%*[^,],%49[^,],%*[^,],%*[^,],%*[^,],%lf\n", username, &cpu_usage) != 2)
    {
      fclose(file);
      return BAD_DATE;
    }

    if (cpu_usage > max_cpu_usage)
    {
      max_cpu_usage = cpu_usage;
      max_cpu_line_number = line_number;
    }
  }

  fclose(file);
  return max_cpu_line_number;
}

double efficiency_score(char *input_file, char *lab)
{
  FILE *file = fopen(input_file, "r");
  if (file == NULL)
  {
    fprintf(stderr, "Error opening file\n");
    return FILE_READ_ERR;
  }

  char line[MAX_LINE_SIZE];
  double total_cpu_usage = 0.0;
  double total_processes = 0.0;
  char file_lab[50];
  double cpu_usage;
  int processes_run;

  while (fgets(line, sizeof(line), file) != NULL)
  {
    if (sscanf(line, "%*[^,],%*[^,],%*[^,],%*[^,],%d,\"%49[^\"]\",%lf\n", &processes_run, file_lab, &cpu_usage) != 3)
    {
      fclose(file);
      return BAD_DATE;
    }

    if (strcmp(file_lab, lab) == 0)
    {
      total_cpu_usage += cpu_usage;
      total_processes += processes_run;
    }
  }

  fclose(file);
  return total_cpu_usage / total_processes;
}

int generate_report(char *input_file, char *output_file, char *lab, int start_date, int end_date)
{
  FILE *infile = fopen(input_file, "r");
  if (infile == NULL)
  {
    fprintf(stderr, "Error opening input file\n");
    return FILE_READ_ERR;
  }

  FILE *outfile = fopen(output_file, "w");
  if (outfile == NULL)
  {
    fprintf(stderr, "Error opening output file\n");
    fclose(infile);
    return FILE_WRITE_ERR;
  }

  char line[MAX_LINE_SIZE];
  int total_logins = 0;
  double total_cpu_minutes = 0.0;
  int total_processes = 0;
  double total_efficiency = 0.0;
  int session_count = 0;
  double session_duration, cpu_usage;
  int processes_run;
  char file_lab[50], top_program[50];
  int file_year, file_month, file_day;
  int date;

  while (fgets(line, sizeof(line), infile) != NULL)
  {
    if (sscanf(line, "%d-%d-%d,\"%49[^\"]\",%*[^,],%lf,%d,\"%49[^\"]\",%lf\n", &file_year, &file_month, &file_day, file_lab, &session_duration, &processes_run, top_program, &cpu_usage) != 8)
    {
      fclose(infile);
      fclose(outfile);
      return BAD_DATE;
    }

    date = file_year * 10000 + file_month * 100 + file_day;
    if (date >= start_date && date <= end_date && strcmp(file_lab, lab) == 0)
    {
      total_logins++;
      total_cpu_minutes += cpu_usage;
      total_processes += processes_run;
      total_efficiency += session_duration - (processes_run * processes_run) / cpu_usage;
      fprintf(outfile, "%s\n", top_program);
      session_count++;
    }
  }

  if (session_count == 0)
  {
    fclose(infile);
    fclose(outfile);
    return NO_DATA;
  }

  fprintf(outfile, "Lab: %s\n", lab);
  fprintf(outfile, "Start date: %04d-%02d-%02d\n", start_date / 10000, (start_date / 100) % 100, start_date % 100);
  fprintf(outfile, "End date: %04d-%02d-%02d\n", end_date / 10000, (end_date / 100) % 100, end_date % 100);
  fprintf(outfile, "Total logins: %d\n", total_logins);
  fprintf(outfile, "Total CPU usage (hours): %.2f\n", total_cpu_minutes / 60.0);
  fprintf(outfile, "Average CPU usage per login (minutes): %.2f\n", total_cpu_minutes / total_logins);
  fprintf(outfile, "Processes executed: %d\n", total_processes);
  fprintf(outfile, "Efficiency score: %.2f\n", total_efficiency / session_count);

  fclose(infile);
  fclose(outfile);
  return SUCCESS;
}
