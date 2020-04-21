#include <stdlib.h>
#include <errno.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <signal.h>

#include "helper_functions.h"
#include "signal_handlers.h"
#include "task.h"

/*
 * "Jobber" job spooler.
 */

int main(int argc, char *argv[])
{
    // TO BE IMPLEMENTED
    jobs_init();


    char *s = "jobber> ";
    int firstSpace = 0;
    char *out = NULL;


    while(1){
        firstSpace = 0;
        int is_first_quote_present, is_last_quote_present, words_in_first_token = 0;
        out = sf_readline(s);

        sigprocmask(SIG_BLOCK,&mask_one, &prev_one);
        if(out == 0){
            free(out);
            jobs_fini();
            exit(EXIT_SUCCESS);
        }

        if(*out == '\0'){
            free(out);
            sigprocmask(SIG_SETMASK, &prev_one, NULL);
            continue;
        }
        char *temp = out;
        while(!(isspace(*temp) || *temp == '\0')){
            temp++;
            firstSpace++;
        }


        char *cmd = malloc((firstSpace+1) * sizeof(char));
        memcpy(cmd,out, firstSpace);
        *(cmd+firstSpace) = '\0';
        if(strcmp(cmd, "help") == 0){
            help_text();
            free(cmd);
            free(out);
        }else if(strcmp(cmd, "spool") == 0){
            char *args = parse_command(cmd, temp, &is_first_quote_present, &is_last_quote_present, &words_in_first_token);
            if(args != NULL){
                char *new_args_copy = malloc(sizeof(char) * (strlen(args)+1));
                strcpy(new_args_copy, args);
                free(cmd);
                free(out);
                job_create(new_args_copy);
                // free(new_args_copy);
            }else{
                free(cmd);
                free(out);
            }
        }else if(strcmp(cmd, "enable") == 0){
            while(isspace(*temp)){
                temp++;
            }
            if(*temp != '\0'){
                printf("Wrong number of args (given: %d, required: 1) for command '%s'\n", 1,cmd);
            }
            free(cmd);
            free(out);
            jobs_set_enabled(1);
        }else if(strcmp(cmd, "disable") == 0){
            while(isspace(*temp)){
                temp++;
            }
            if(*temp != '\0'){
                printf("Wrong number of args (given: %d, required: 1) for command '%s'\n", 1,cmd);
            }
            free(cmd);
            free(out);
            jobs_set_enabled(0);
        }else if(strcmp(cmd, "pause") == 0){
            char *args = parse_command(cmd, temp, &is_first_quote_present, &is_last_quote_present, &words_in_first_token);
            if(args != NULL){
                if(strlen(args)>1){
                    printf("Error:%s\n", cmd);
                }else{
                    int jobid = *args - '0';
                    job_pause(jobid);
                }
            }
            free(cmd);
            free(out);
        }else if(strcmp(cmd, "resume") == 0){
            char *args = parse_command(cmd, temp, &is_first_quote_present, &is_last_quote_present, &words_in_first_token);
            if(args != NULL){
                if(strlen(args)>1){
                    printf("Error:%s\n", cmd);
                }else{
                    int jobid = *args - '0';
                    job_resume(jobid);
                }
            }
            free(cmd);
            free(out);
        }else if(strcmp(cmd, "cancel") == 0){
            char *args = parse_command(cmd, temp, &is_first_quote_present, &is_last_quote_present, &words_in_first_token);
            if(args != NULL){
                if(strlen(args)>1){
                    printf("Error:%s\n", cmd);
                }else{
                    int jobid = *args - '0';
                    job_cancel(jobid);
                }
            }
            free(cmd);
            free(out);
        }else if(strcmp(cmd, "jobs") == 0){
            print_jobs();
            free(cmd);
            free(out);
        }else if(strcmp(cmd, "expunge") == 0){
            char *args = parse_command(cmd, temp, &is_first_quote_present, &is_last_quote_present, &words_in_first_token);
            if(args != NULL){
                if(strlen(args)>1){
                    printf("Error:%s\n", cmd);
                }else{
                    int jobid = *args - '0';
                    job_expunge(jobid);
                }
            }
            free(cmd);
            free(out);
        }else if(strcmp(cmd, "status") == 0){
            char *args = parse_command(cmd, temp, &is_first_quote_present, &is_last_quote_present, &words_in_first_token);
            if(args != NULL){
                if(strlen(args)>1){
                    printf("Error:%s\n", cmd);
                }else{
                    int jobid = *args - '0';
                    print_job(jobid);
                }
            }
            free(cmd);
            free(out);
        }else if(strcmp(cmd, "quit") == 0){
            free(cmd);
            free(out);
            jobs_fini();
            exit(EXIT_SUCCESS);
        }else{
            printf("Unrecognized command: %s\n",cmd);
            free(cmd);
            free(out);
        }

        // free(cmd);
        // free(out);
        sigprocmask(SIG_SETMASK, &prev_one, NULL);
    }

    exit(EXIT_FAILURE);
}

/*
 * Just a reminder: All non-main functions should
 * be in another file not named main.c
 */
