#include <unistd.h>

#include "jobber.h"


typedef struct JOB_ITEM{
    char *command;
    pid_t pgid;
    JOB_STATUS status;
    int is_free;
    int exit_status;
    int is_canceled;
}JOB_ITEM;

int active_runners;

struct JOB_ITEM jobs_table[8];

void help_text();

void print_jobs();

void print_job(int jobid);

void fill_table_row(int i, char *command,JOB_STATUS status);

void set_table_row_status(int i, JOB_STATUS new_status);

void set_table_row_pgid(int i,pid_t pgid);

void run_task();

char *parse_command(char *cmd, char *args, int *is_first_quote_present, int *is_last_quote_present, int *words_in_first_token);

// int check_runners();