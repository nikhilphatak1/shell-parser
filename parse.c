/*
Nikhil Phatak
CS3650
Prof Tuck
HW04
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "tokenize.h"

int
is_comb_op(char* op) {
    int semi_comp = strcmp(op,";");
    int comma_comp = strcmp(op,",");
    int amper_comp = strcmp(op,"&");
    int amper_2_comp = strcmp(op,"&&");
    int pipe_comp = strcmp(op,"|");
    int pipe_2_comp = strcmp(op,"||");

    return (!semi_comp || !comma_comp || !amper_comp ||
            !amper_2_comp || !pipe_comp || !pipe_2_comp);
}

int
is_redir_op(char* op) {
    return (!strcmp(op,"<") || !strcmp(op,">"));
}

void
free_str_array(char** strs) {
    for (int idx = 0; idx < 20; idx++) {
        free(strs[idx]);
    }
    free(strs);
}

// returns index of op in xs
// if op is not in xs, return -1
int
contains_op(char** xs, char* op, int left, int right) {
    int idx = left;
    while (idx < right) {
        if (!strcmp(xs[idx],op)) {
            return idx;
        }
        ++idx;
    }
    return -1;
}

void
quote_wrap(char* to_print, char* target, int space_before, int space_after) {
    if (space_before) {
        strcat(to_print," \"");
    } else {
        strcat(to_print,"\"");
    }
    strcat(to_print,target);
    if (space_after) {
        strcat(to_print,"\" ");
    } else {
        strcat(to_print,"\"");
    }
}

char*
build_string(char** xs, int left, int right) {

    char* to_print = malloc(200*sizeof(char));
    strcpy(to_print,"(");

    // identify the operator in the token list
    int power_token_index = -1;
    for (int i = left; i < right; ++i) {
        if (is_comb_op(xs[i]) || is_redir_op(xs[i])) {
            power_token_index = i;
        }
    }

    int redirect = 0;
    if (power_token_index == -1) {
        int j;
        for (j = left; j < right-1; ++j) {
            quote_wrap(to_print,xs[j],0,1);
        }
        quote_wrap(to_print,xs[j],0,0);

    } else {
        if (contains_op(xs,"<",left,right) != -1) {
            redirect = 1;
            power_token_index = contains_op(xs,"<",left,right);
        } else if (contains_op(xs,">",left,right) != -1) {
            redirect = 1;
            power_token_index = contains_op(xs,">",left,right);
        } else if (contains_op(xs,";",left,right) != -1) {
            power_token_index = contains_op(xs,";",left,right);
        } else if (contains_op(xs,",",left,right) != -1) {
            power_token_index = contains_op(xs,",",left,right);
        } else if (contains_op(xs,"&",left,right) != -1) {
            power_token_index = contains_op(xs,"&",left,right);
        } else if (contains_op(xs,"||",left,right) != -1) {
            power_token_index = contains_op(xs,"||",left,right);
        } else if (contains_op(xs,"&&",left,right) != -1) {
            power_token_index = contains_op(xs,"&&",left,right);
        } else if (contains_op(xs,"|",left,right) != -1) {
            power_token_index = contains_op(xs,"|",left,right);
        }

        // "token"
        quote_wrap(to_print,xs[power_token_index],0,1);

        // "token1" "token2"
        char* returned_string_1 = build_string(xs,left,power_token_index);
        strcat(to_print,returned_string_1);
        free(returned_string_1);

        // if redirection operator, dont wrap right argument
        // in parens
        if (redirect) {
            quote_wrap(to_print,xs[power_token_index+1],1,0);

        } else {
            if (power_token_index+1 != right) {
                strcat(to_print," ");
                char* returned_string_2 = build_string(xs,power_token_index+1,right);
                strcat(to_print,returned_string_2);
                free(returned_string_2);
            }
        }
    }

    strcat(to_print,")");
    return to_print;
}

void
build_string_wrapper(char** xs) {

    int idx = 0;
    while (strcmp(xs[idx]," ")) {
        ++idx;
    }
    char* result = build_string(xs,0,idx);
    printf("%s\n",result);
    free(result);
}

int
main(int argc, char* argv[])
{
    char line[128];
    FILE* input = stdin;

    if (argc == 2) {
        input = fopen(argv[1], "r");
    }

    while (1) {
        if (argc == 1) {
            printf("parse$ ");
            fflush(stdout);
        }

        char* rv = fgets(line, 100, input);
        if (!rv) {
            break;
        }
        char** xs = tokenize(line);
        build_string_wrapper(xs);
        free_str_array(xs);
    }

    return 0;
}
