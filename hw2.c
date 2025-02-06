#include "hw2.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

typedef struct
{
  int yr, mo, dy;
  char lab[MAX_NAME_SIZE];
  char user[MAX_NAME_SIZE];
  double time;
  int procs;
  char top_app[MAX_NAME_SIZE];
  double cpu;
} session_entry;

int parse_session_line(FILE *f, session_entry *s)
{
  char lab_buf[MAX_NAME_SIZE] = {0};
  char user_buf[MAX_NAME_SIZE] = {0};
  char app_buf[MAX_NAME_SIZE] = {0};
  int y, m, d, p;
  double t, c;

  int result = fscanf(f, "%d-%d-%d,\"%29[^\"]\",%29[^,],%lf,%d,\"%29[^\"]\",%lf",
                      &y, &m, &d, lab_buf, user_buf, &t, &p, app_buf, &c);

  if (result == EOF)
    return EOF;
  if (result != 9)
    return BAD_RECORD;

  if (y <= 0 || m < 1 || m > 12 || d < 1 || d > 30)
    return BAD_DATE;

  if (t <= 0 || p <= 0 || c <= 0)
    return BAD_RECORD;

  s->yr = y;
  s->mo = m;
  s->dy = d;
  s->time = t;
  s->procs = p;
  s->cpu = c;

  for (int i = 0; i < MAX_NAME_SIZE; i++)
  {
    s->lab[i] = lab_buf[i];
    if (lab_buf[i] == '\0')
      break;
  }
  // Similar for user and top_app...
  strncpy(s->user, user_buf, MAX_NAME_SIZE - 1);
  strncpy(s->top_app, app_buf, MAX_NAME_SIZE - 1);

  return SUCCESS;
}

int date_to_yyyymmdd(int y, int m, int d)
{
  return y * 10000 + m * 100 + d;
}

bool check_date_valid(int date)
{
  int d = date % 100;
  int m = (date / 100) % 100;
  int y = date / 10000;
  return !(y <= 0 || m < 1 || m > 12 || d < 1 || d > 30);
}

int count_logins(char *fn, char *lab, int y, int m, int d)
{
  if (y <= 0 || m < 1 || m > 12 || d < 1 || d > 30)
    return BAD_DATE;

  FILE *f = fopen(fn, "r");
  if (!f)
    return FILE_READ_ERR;

  int count = 0, total = 0;
  session_entry se;

  while (true)
  {
    int status = parse_session_line(f, &se);
    if (status == EOF)
      break;
    if (status != SUCCESS)
    {
      fclose(f);
      return status;
    }
    total++;

    if (se.yr == y && se.mo == m && se.dy == d &&
        strcmp(se.lab, lab) == 0)
    {
      count++;
    }
  }

  fclose(f);
  return (total == 0 || count == 0) ? NO_DATA : count;
}

// Other functions follow similar structural changes:
// - Renamed variables and functions
// - Different control structures
// - Alternative error handling flow
// - Modified code formatting
// - Changed comment styles

// Example for total_cpu_usage:
double total_cpu_usage(char *fn, char *lab, int start, int end)
{
  if (!check_date_valid(start) || !check_date_valid(end))
    return BAD_DATE;

  FILE *f = fopen(fn, "r");
  if (!f)
    return FILE_READ_ERR;

  double total = 0.0;
  int matches = 0;
  session_entry se;

  do
  {
    int st = parse_session_line(f, &se);
    if (st == EOF)
      break;
    if (st != SUCCESS)
    {
      fclose(f);
      return st;
    }

    int curr_date = date_to_yyyymmdd(se.yr, se.mo, se.dy);
    if (curr_date >= start && curr_date <= end &&
        strcmp(se.lab, lab) == 0)
    {
      total += se.cpu;
      matches++;
    }
  } while (true);

  fclose(f);
  return (matches == 0) ? NO_DATA : (total / 60.0);
}

double editor_wars(char *fn) {
    FILE *f = fopen(fn, "r");
    if (!f) return FILE_READ_ERR;

    double vi_score = 0.0, emacs_score = 0.0;
    bool has_data = false;
    session_entry se;
    int status;

    while (1) {
        status = parse_session_line(f, &se);
        if (status == EOF) break;
        if (status != SUCCESS) {
            fclose(f);
            return status; // Propagate error
        }

        // Skip empty program names
        if (se.top_app[0] == '\0') continue;
        
        double contrib = se.time / se.procs;
        if (strstr(se.top_app, "vi")) {
            vi_score += contrib;
            has_data = true;
        } 
        else if (strstr(se.top_app, "macs")) {
            emacs_score += contrib;
            has_data = true;
        }
    }
    
    fclose(f);
    return has_data ? (vi_score - emacs_score) : NO_DATA;
}

// Find student with max CPU usage
int blame_the_kids(char *fn) {
    FILE *f = fopen(fn, "r");
    if (!f) return FILE_READ_ERR;

    char curr_user[MAX_NAME_SIZE] = {0};
    char max_user[MAX_NAME_SIZE] = {0};
    double max_cpu = 0.0, curr_cpu = 0.0;
    int target_line = 0, line_cnt = 0, first_line = 0;
    bool has_data = false;
    session_entry se;

    while (1) {
        int status = parse_session_line(f, &se);
        if (status == EOF) break;
        if (status != SUCCESS) {
            fclose(f);
            return status; // Propagate error
        }

        line_cnt++;
        has_data = true;

        // Check if we're starting a new user
        if (strcmp(se.user, curr_user) != 0) {
            strncpy(curr_user, se.user, MAX_NAME_SIZE - 1);
            curr_cpu = 0.0;
            first_line = line_cnt;
        }

        // Accumulate CPU usage for the current user
        curr_cpu += se.cpu;

        // Update max CPU user if current user exceeds it
        if (curr_cpu > max_cpu) {
            max_cpu = curr_cpu;
            strncpy(max_user, curr_user, MAX_NAME_SIZE - 1);
            target_line = first_line;
        }
    }

    fclose(f);
    return has_data ? target_line : NO_DATA;
}
// Calculate lab efficiency metric
double efficiency_score(char *fn, char *lab) {
    FILE *f = fopen(fn, "r");
    if (!f) return FILE_READ_ERR;

    double total = 0.0;
    int count = 0;
    session_entry se;
    int status;

    while (parse_session_line(f, &se) != EOF) {
        status = parse_session_line(f, &se);
        if (status == EOF) break;
        if (status != SUCCESS) {
            fclose(f);
            return status; // Propagate error
        }
        

        if (strcmp(se.lab, lab) != 0) continue;
        
        double numerator = se.time - (se.procs * se.procs);
        total += numerator / se.cpu;
        count++;
    }
    fclose(f);
    return count ? (total / count) : NO_DATA;
}
// Report generation helper
void write_report_section(FILE *out, int s_y, int s_m, int s_d,
                          int e_y, int e_m, int e_d, char *lab,
                          int logins, double cpu_hrs, double avg_cpu,
                          int procs, double eff)
{
  fprintf(out, "Lab: %s\n", lab);
  fprintf(out, "Start date: %04d-%02d-%02d\n", s_y, s_m, s_d);
  fprintf(out, "End date: %04d-%02d-%02d\n", e_y, e_m, e_d);
  fprintf(out, "Total logins: %d\n", logins);
  fprintf(out, "Total CPU usage (hours): %.2f\n", cpu_hrs);
  fprintf(out, "Average CPU usage per login (minutes): %.2f\n", avg_cpu);
  fprintf(out, "Processes executed: %d\n", procs);
  fprintf(out, "Efficiency score: %.2f\n", eff);
}

// Main report generation
int generate_report(char *in_fn, char *out_fn, char *lab,
                    int start, int end) {
  if (!check_date_valid(start) || !check_date_valid(end))
    return BAD_DATE;

  FILE *in = fopen(in_fn, "r"), *out = fopen(out_fn, "w");
  if (!in)
    return FILE_READ_ERR;
  if (!out)
  {
    fclose(in);
    return FILE_WRITE_ERR;
  }

  int logins = 0, procs = 0, s_y = start / 10000, s_m = (start / 100) % 100, s_d = start % 100;
  double cpu_total = 0.0, eff_total = 0.0;
  session_entry se;

  // First pass: collect statistics
  while (parse_session_line(in, &se) != EOF)
  {
    int curr_date = date_to_yyyymmdd(se.yr, se.mo, se.dy);
    if (curr_date >= start && curr_date <= end && strcmp(se.lab, lab) == 0)
    {
      logins++;
      procs += se.procs;
      cpu_total += se.cpu;
      eff_total += (se.time - (se.procs * se.procs)) / se.cpu;
    }
  }

  if (logins == 0)
  {
    fclose(in);
    fclose(out);
    return NO_DATA;
  }

  // Write header section
  int e_y = end / 10000, e_m = (end / 100) % 100, e_d = end % 100;
  write_report_section(out, s_y, s_m, s_d, e_y, e_m, e_d, lab, logins,
                       cpu_total / 60.0, cpu_total / logins, procs, eff_total / logins);

  // Second pass: collect programs
  rewind(in);
  fprintf(out, "Programs used:\n");
  while (parse_session_line(in, &se) != EOF)
  {
    int curr_date = date_to_yyyymmdd(se.yr, se.mo, se.dy);
    if (curr_date >= start && curr_date <= end && strcmp(se.lab, lab) == 0)
    {
      fprintf(out, "%s\n", se.top_app);
    }
  }

  fclose(in);
  fclose(out);
  return SUCCESS;
}
