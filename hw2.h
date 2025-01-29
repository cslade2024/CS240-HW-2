#ifndef HW2_H
#define HW2_H

/* Constant Definitions */

#define MAX_NAME_SIZE (30)

/* Error Codes */

#define SUCCESS (0)
#define FILE_READ_ERR (-1)
#define FILE_WRITE_ERR (-2)
#define BAD_RECORD (-3)
#define BAD_DATE (-4)
#define NO_DATA (-5)

/* You are not allowed to use these functions */

#define access(x, y) (printf("\n\n\n\n\nDo not use access().\n"), abort(), 1)

/* Function Prototypes */

int count_logins(char *input_file, char *lab, int year, int month, int day);
double total_cpu_usage(char *input_file, char *lab, int start_date,
                       int end_date);
double editor_wars(char *input_file);
int blame_the_kids(char *input_file);
double efficiency_score(char *input_file, char *lab);
int generate_report(char *input_file, char *output_file, char *lab,
                    int start_date, int end_date);

#endif // HW2_H
